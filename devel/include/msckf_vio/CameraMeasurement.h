// Generated by gencpp from file msckf_vio/CameraMeasurement.msg
// DO NOT EDIT!


#ifndef MSCKF_VIO_MESSAGE_CAMERAMEASUREMENT_H
#define MSCKF_VIO_MESSAGE_CAMERAMEASUREMENT_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>
#include <msckf_vio/FeatureMeasurement.h>

namespace msckf_vio
{
template <class ContainerAllocator>
struct CameraMeasurement_
{
  typedef CameraMeasurement_<ContainerAllocator> Type;

  CameraMeasurement_()
    : header()
    , features()  {
    }
  CameraMeasurement_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , features(_alloc)  {
  (void)_alloc;
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef std::vector< ::msckf_vio::FeatureMeasurement_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::msckf_vio::FeatureMeasurement_<ContainerAllocator> >::other >  _features_type;
  _features_type features;





  typedef boost::shared_ptr< ::msckf_vio::CameraMeasurement_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::msckf_vio::CameraMeasurement_<ContainerAllocator> const> ConstPtr;

}; // struct CameraMeasurement_

typedef ::msckf_vio::CameraMeasurement_<std::allocator<void> > CameraMeasurement;

typedef boost::shared_ptr< ::msckf_vio::CameraMeasurement > CameraMeasurementPtr;
typedef boost::shared_ptr< ::msckf_vio::CameraMeasurement const> CameraMeasurementConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::msckf_vio::CameraMeasurement_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::msckf_vio::CameraMeasurement_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace msckf_vio

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': True}
// {'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg'], 'msckf_vio': ['/home/vtkc/Desktop/tlab/msckf_loopClosure_NoLoop/src/msckf_vio/msg'], 'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::msckf_vio::CameraMeasurement_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::msckf_vio::CameraMeasurement_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::msckf_vio::CameraMeasurement_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::msckf_vio::CameraMeasurement_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::msckf_vio::CameraMeasurement_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::msckf_vio::CameraMeasurement_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::msckf_vio::CameraMeasurement_<ContainerAllocator> >
{
  static const char* value()
  {
    return "49e471535134556a8117bc01f9d9b57d";
  }

  static const char* value(const ::msckf_vio::CameraMeasurement_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x49e471535134556aULL;
  static const uint64_t static_value2 = 0x8117bc01f9d9b57dULL;
};

template<class ContainerAllocator>
struct DataType< ::msckf_vio::CameraMeasurement_<ContainerAllocator> >
{
  static const char* value()
  {
    return "msckf_vio/CameraMeasurement";
  }

  static const char* value(const ::msckf_vio::CameraMeasurement_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::msckf_vio::CameraMeasurement_<ContainerAllocator> >
{
  static const char* value()
  {
    return "std_msgs/Header header\n\
# All features on the current image,\n\
# including tracked ones and newly detected ones.\n\
FeatureMeasurement[] features\n\
\n\
================================================================================\n\
MSG: std_msgs/Header\n\
# Standard metadata for higher-level stamped data types.\n\
# This is generally used to communicate timestamped data \n\
# in a particular coordinate frame.\n\
# \n\
# sequence ID: consecutively increasing ID \n\
uint32 seq\n\
#Two-integer timestamp that is expressed as:\n\
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n\
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n\
# time-handling sugar is provided by the client library\n\
time stamp\n\
#Frame this data is associated with\n\
# 0: no frame\n\
# 1: global frame\n\
string frame_id\n\
\n\
================================================================================\n\
MSG: msckf_vio/FeatureMeasurement\n\
uint64 id\n\
# Normalized feature coordinates (with identity intrinsic matrix)\n\
float64 u0 # horizontal coordinate in cam0\n\
float64 v0 # vertical coordinate in cam0\n\
float64 u1 # horizontal coordinate in cam0\n\
float64 v1 # vertical coordinate in cam0\n\
";
  }

  static const char* value(const ::msckf_vio::CameraMeasurement_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::msckf_vio::CameraMeasurement_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.features);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct CameraMeasurement_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::msckf_vio::CameraMeasurement_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::msckf_vio::CameraMeasurement_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "features[]" << std::endl;
    for (size_t i = 0; i < v.features.size(); ++i)
    {
      s << indent << "  features[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::msckf_vio::FeatureMeasurement_<ContainerAllocator> >::stream(s, indent + "    ", v.features[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // MSCKF_VIO_MESSAGE_CAMERAMEASUREMENT_H
