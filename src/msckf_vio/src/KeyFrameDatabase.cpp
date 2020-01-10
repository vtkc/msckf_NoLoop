#include <msckf_vio/KeyFrameDatabase.h>

#include <msckf_vio/KeyFrame.h>
#include <../Thirdparty/DBoW2/DBoW2/BowVector.h>

#include<mutex>

using namespace std;

namespace msckf_vio{
    KeyFrameDatabase::KeyFrameDatabase (const ORBVocabulary &voc):
        mpVoc(&voc)
    {
        mvInvertedFile.resize(voc.size());
    }


    void KeyFrameDatabase::add(KeyFrame *pKF)
    {
        unique_lock<mutex> lock(mMutex);

        for(DBoW2::BowVector::const_iterator vit= pKF->mBowVec.begin(), vend=pKF->mBowVec.end(); vit!=vend; vit++)
            mvInvertedFile[vit->first].push_back(pKF);
    }

    void KeyFrameDatabase::erase(KeyFrame* pKF)
    {
        unique_lock<mutex> lock(mMutex);

        // Erase elements in the Inverse File for the entry
        for(DBoW2::BowVector::const_iterator vit=pKF->mBowVec.begin(), vend=pKF->mBowVec.end(); vit!=vend; vit++)
        {
            // List of keyframes that share the word
            list<KeyFrame*> &lKFs =   mvInvertedFile[vit->first];

            for(list<KeyFrame*>::iterator lit=lKFs.begin(), lend= lKFs.end(); lit!=lend; lit++)
            {
                if(pKF==*lit)
                {
                    lKFs.erase(lit);
                    break;
                }
            }
        }
    }

    void KeyFrameDatabase::clear()
    {
        mvInvertedFile.clear();
        mvInvertedFile.resize(mpVoc->size());
    }

    vector<list<KeyFrame*>> KeyFrameDatabase::getKFDB()
    {
        return mvInvertedFile;
    }

    void KeyFrameDatabase::clearKFDB()
    {
        mvInvertedFile.clear();
    }


    vector<KeyFrame*> KeyFrameDatabase::DetectLoopCandidates(KeyFrame* pKF, float minScore)
    {
        // cout << "Triggered DetectLoopCandidates()\n" << endl;
        set<KeyFrame*> spConnectedKeyFrames = pKF->GetConnectedKeyFrames();
        list<KeyFrame*> lKFsSharingWords;

        // Search all keyframes that share a word with current keyframes
        // Discard keyframes connected to the query keyframe
        {
            unique_lock<mutex> lock(mMutex);
            
            // cout << "Checking all KF that shares a word with current KF." << endl;
            for(DBoW2::BowVector::const_iterator vit=pKF->mBowVec.begin(), vend=pKF->mBowVec.end(); vit != vend; vit++)
            {
                list<KeyFrame*> &lKFs =   mvInvertedFile[vit->first];

                for(list<KeyFrame*>::iterator lit=lKFs.begin(), lend= lKFs.end(); lit!=lend; lit++)
                {
                    KeyFrame* pKFi=*lit;
                    if(pKFi->mnLoopQuery!=pKF->mnId)
                    {
                        pKFi->mnLoopWords=0;
                        if(!spConnectedKeyFrames.count(pKFi))
                        {
                            // cout << "A KF shares at least a word with Candidate KF." << endl;
                            pKFi->mnLoopQuery=pKF->mnId;
                            lKFsSharingWords.push_back(pKFi);
                        }
                    }
                    pKFi->mnLoopWords++;
                }
            }
        }
        // cout << "Number of covisible KF that shares at least 1 common word with current KF = " << lKFsSharingWords.size() << endl;
        // cout << "Finish checking.\n" << endl;

        if(lKFsSharingWords.empty()){
            // cout << "lKFsSharingWords is empty." << endl;
            // cout << "Returning vector<KeyFrame*>() and END of DetectLoopCandidates().\n\n" << endl;
            return vector<KeyFrame*>();
        }

        list<pair<float,KeyFrame*> > lScoreAndMatch;

        // Only compare against those keyframes that share enough words
        int maxCommonWords=0;
        // cout << "Filtering covisible KF that share enough words." << endl;
        for(list<KeyFrame*>::iterator lit=lKFsSharingWords.begin(), lend= lKFsSharingWords.end(); lit!=lend; lit++)
        {
            if((*lit)->mnLoopWords>maxCommonWords){
                // cout << "There is one covisible KF that shares enough words." << endl;
                maxCommonWords=(*lit)->mnLoopWords;
            }
        }
        // cout << "maxCommonWords = " << maxCommonWords << endl;
        // cout << "End of filtering.\n" << endl;

        int minCommonWords = maxCommonWords*0.8f;

        // cout << "minCommonWords = " << minCommonWords << endl;

        int nscores=0;

        // Compute similarity score. Retain the matches whose score is higher than minScore
        // cout << "Computing Similarity Score and retain matches whose score is higher than minScore" << endl;
        for(list<KeyFrame*>::iterator lit=lKFsSharingWords.begin(), lend= lKFsSharingWords.end(); lit!=lend; lit++)
        {
            KeyFrame* pKFi = *lit;

            if(pKFi->mnLoopWords>minCommonWords)
            {
                nscores++;

                float si = mpVoc->score(pKF->mBowVec,pKFi->mBowVec);

                pKFi->mLoopScore = si;
                if(si>=minScore){
                    lScoreAndMatch.push_back(make_pair(si,pKFi));
                    // cout << "KF[" << pKFi->mnId <<"] whose score is higher than minScore. KF Score = " << si << " minScore = " << minScore << endl;
                }
            }
        }
        // cout << "Number of retained matches = " << lScoreAndMatch.size() << endl;
        // cout << "End of calculating and retaining matches whose score is higher than minScore.\n" << endl;

        if(lScoreAndMatch.empty()){
            // cout << "There are no matches that are higher than minScore.\nEnding DetectLoopCandidates().\n\n" << endl;
            return vector<KeyFrame*>();
        }

        list<pair<float,KeyFrame*> > lAccScoreAndMatch;
        float bestAccScore = minScore;

        // Lets now accumulate score by covisibility
        // cout << "Accumilating score by covisibility." << endl;
        for(list<pair<float,KeyFrame*> >::iterator it=lScoreAndMatch.begin(), itend=lScoreAndMatch.end(); it!=itend; it++)
        {
            // Gets the KF from the Pair  // 1st variable is the score in float, 2nd variable is the matched KF
            KeyFrame* pKFi = it->second;
            // Get the 10 best covisibility KF that sees the same spatial points observed by matched KF
            vector<KeyFrame*> vpNeighs = pKFi->GetBestCovisibilityKeyFrames(10);

            float bestScore = it->first;
            float accScore = it->first;
            KeyFrame* pBestKF = pKFi;
            for(vector<KeyFrame*>::iterator vit=vpNeighs.begin(), vend=vpNeighs.end(); vit!=vend; vit++)
            {
                KeyFrame* pKF2 = *vit;
                if(pKF2->mnLoopQuery==pKF->mnId && pKF2->mnLoopWords>minCommonWords)
                {
                    accScore+=pKF2->mLoopScore;
                    if(pKF2->mLoopScore>bestScore)
                    {
                        pBestKF=pKF2;
                        bestScore = pKF2->mLoopScore;
                    }
                }
            }

            lAccScoreAndMatch.push_back(make_pair(accScore,pBestKF));
            if(accScore>bestAccScore)
                bestAccScore=accScore;
        }
        // cout << "Finished Accumulating score.\n" << endl;

        // Return all those keyframes with a score higher than 0.75*bestScore
        float minScoreToRetain = 0.75f*bestAccScore;

        set<KeyFrame*> spAlreadyAddedKF;
        vector<KeyFrame*> vpLoopCandidates;
        vpLoopCandidates.reserve(lAccScoreAndMatch.size());

        // cout << "Returning all KF with score higher than " << minScoreToRetain << endl;
        for(list<pair<float,KeyFrame*> >::iterator it=lAccScoreAndMatch.begin(), itend=lAccScoreAndMatch.end(); it!=itend; it++)
        {
            if(it->first>minScoreToRetain)
            {
                KeyFrame* pKFi = it->second;
                if(!spAlreadyAddedKF.count(pKFi))
                {
                    // cout << "KF[" << pKFi->mnId <<"] is higher than " << minScoreToRetain << " with score = " << it->first << endl;
                    vpLoopCandidates.push_back(pKFi);
                    spAlreadyAddedKF.insert(pKFi);
                }
            }
        }
        // cout << "Potential loop candidate KF Array size = " << vpLoopCandidates.size() << endl;
        // cout << "Returned all potential loop candidate KFs and END of DetectLoopCandidates().\n\n" << endl;


        return vpLoopCandidates;
    }

    vector<KeyFrame*> KeyFrameDatabase::DetectRelocalizationCandidates(Frame *F)
    {
        list<KeyFrame*> lKFsSharingWords;

        // Search all keyframes that share a word with current frame
        {
            unique_lock<mutex> lock(mMutex);

            for(DBoW2::BowVector::const_iterator vit=F->mBowVec.begin(), vend=F->mBowVec.end(); vit != vend; vit++)
            {
                list<KeyFrame*> &lKFs =   mvInvertedFile[vit->first];

                for(list<KeyFrame*>::iterator lit=lKFs.begin(), lend= lKFs.end(); lit!=lend; lit++)
                {
                    KeyFrame* pKFi=*lit;
                    if(pKFi->mnRelocQuery!=F->mnId)
                    {
                        pKFi->mnRelocWords=0;
                        pKFi->mnRelocQuery=F->mnId;
                        lKFsSharingWords.push_back(pKFi);
                    }
                    pKFi->mnRelocWords++;
                }
            }
        }
        if(lKFsSharingWords.empty())
            return vector<KeyFrame*>();

        // Only compare against those keyframes that share enough words
        int maxCommonWords=0;
        for(list<KeyFrame*>::iterator lit=lKFsSharingWords.begin(), lend= lKFsSharingWords.end(); lit!=lend; lit++)
        {
            if((*lit)->mnRelocWords>maxCommonWords)
                maxCommonWords=(*lit)->mnRelocWords;
        }

        int minCommonWords = maxCommonWords*0.8f;

        list<pair<float,KeyFrame*> > lScoreAndMatch;

        int nscores=0;

        // Compute similarity score.
        for(list<KeyFrame*>::iterator lit=lKFsSharingWords.begin(), lend= lKFsSharingWords.end(); lit!=lend; lit++)
        {
            KeyFrame* pKFi = *lit;

            if(pKFi->mnRelocWords>minCommonWords)
            {
                nscores++;
                float si = mpVoc->score(F->mBowVec,pKFi->mBowVec);
                pKFi->mRelocScore=si;
                lScoreAndMatch.push_back(make_pair(si,pKFi));
            }
        }

        if(lScoreAndMatch.empty())
            return vector<KeyFrame*>();

        list<pair<float,KeyFrame*> > lAccScoreAndMatch;
        float bestAccScore = 0;

        // Lets now accumulate score by covisibility
        for(list<pair<float,KeyFrame*> >::iterator it=lScoreAndMatch.begin(), itend=lScoreAndMatch.end(); it!=itend; it++)
        {
            KeyFrame* pKFi = it->second;
            vector<KeyFrame*> vpNeighs = pKFi->GetBestCovisibilityKeyFrames(10);

            float bestScore = it->first;
            float accScore = bestScore;
            KeyFrame* pBestKF = pKFi;
            for(vector<KeyFrame*>::iterator vit=vpNeighs.begin(), vend=vpNeighs.end(); vit!=vend; vit++)
            {
                KeyFrame* pKF2 = *vit;
                if(pKF2->mnRelocQuery!=F->mnId)
                    continue;

                accScore+=pKF2->mRelocScore;
                if(pKF2->mRelocScore>bestScore)
                {
                    pBestKF=pKF2;
                    bestScore = pKF2->mRelocScore;
                }

            }
            lAccScoreAndMatch.push_back(make_pair(accScore,pBestKF));
            if(accScore>bestAccScore)
                bestAccScore=accScore;
        }

        // Return all those keyframes with a score higher than 0.75*bestScore
        float minScoreToRetain = 0.75f*bestAccScore;
        set<KeyFrame*> spAlreadyAddedKF;
        vector<KeyFrame*> vpRelocCandidates;
        vpRelocCandidates.reserve(lAccScoreAndMatch.size());
        for(list<pair<float,KeyFrame*> >::iterator it=lAccScoreAndMatch.begin(), itend=lAccScoreAndMatch.end(); it!=itend; it++)
        {
            const float &si = it->first;
            if(si>minScoreToRetain)
            {
                KeyFrame* pKFi = it->second;
                if(!spAlreadyAddedKF.count(pKFi))
                {
                    vpRelocCandidates.push_back(pKFi);
                    spAlreadyAddedKF.insert(pKFi);
                }
            }
        }

        return vpRelocCandidates;
    }
}