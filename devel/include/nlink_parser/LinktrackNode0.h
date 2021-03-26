// Generated by gencpp from file nlink_parser/LinktrackNode0.msg
// DO NOT EDIT!


#ifndef NLINK_PARSER_MESSAGE_LINKTRACKNODE0_H
#define NLINK_PARSER_MESSAGE_LINKTRACKNODE0_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace nlink_parser
{
template <class ContainerAllocator>
struct LinktrackNode0_
{
  typedef LinktrackNode0_<ContainerAllocator> Type;

  LinktrackNode0_()
    : role(0)
    , id(0)
    , data()  {
    }
  LinktrackNode0_(const ContainerAllocator& _alloc)
    : role(0)
    , id(0)
    , data(_alloc)  {
  (void)_alloc;
    }



   typedef uint8_t _role_type;
  _role_type role;

   typedef uint8_t _id_type;
  _id_type id;

   typedef std::vector<uint8_t, typename ContainerAllocator::template rebind<uint8_t>::other >  _data_type;
  _data_type data;




  typedef boost::shared_ptr< ::nlink_parser::LinktrackNode0_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::nlink_parser::LinktrackNode0_<ContainerAllocator> const> ConstPtr;

}; // struct LinktrackNode0_

typedef ::nlink_parser::LinktrackNode0_<std::allocator<void> > LinktrackNode0;

typedef boost::shared_ptr< ::nlink_parser::LinktrackNode0 > LinktrackNode0Ptr;
typedef boost::shared_ptr< ::nlink_parser::LinktrackNode0 const> LinktrackNode0ConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::nlink_parser::LinktrackNode0_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::nlink_parser::LinktrackNode0_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace nlink_parser

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'nlink_parser': ['/home/kinetic/xrobot_ws/src/nlink_parser/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::nlink_parser::LinktrackNode0_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::nlink_parser::LinktrackNode0_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::nlink_parser::LinktrackNode0_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::nlink_parser::LinktrackNode0_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::nlink_parser::LinktrackNode0_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::nlink_parser::LinktrackNode0_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::nlink_parser::LinktrackNode0_<ContainerAllocator> >
{
  static const char* value()
  {
    return "039fd4e4d2076d4b4621aa4a72ea3366";
  }

  static const char* value(const ::nlink_parser::LinktrackNode0_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x039fd4e4d2076d4bULL;
  static const uint64_t static_value2 = 0x4621aa4a72ea3366ULL;
};

template<class ContainerAllocator>
struct DataType< ::nlink_parser::LinktrackNode0_<ContainerAllocator> >
{
  static const char* value()
  {
    return "nlink_parser/LinktrackNode0";
  }

  static const char* value(const ::nlink_parser::LinktrackNode0_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::nlink_parser::LinktrackNode0_<ContainerAllocator> >
{
  static const char* value()
  {
    return "uint8 role\n\
uint8 id\n\
uint8[] data\n\
";
  }

  static const char* value(const ::nlink_parser::LinktrackNode0_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::nlink_parser::LinktrackNode0_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.role);
      stream.next(m.id);
      stream.next(m.data);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct LinktrackNode0_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::nlink_parser::LinktrackNode0_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::nlink_parser::LinktrackNode0_<ContainerAllocator>& v)
  {
    s << indent << "role: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.role);
    s << indent << "id: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.id);
    s << indent << "data[]" << std::endl;
    for (size_t i = 0; i < v.data.size(); ++i)
    {
      s << indent << "  data[" << i << "]: ";
      Printer<uint8_t>::stream(s, indent + "  ", v.data[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // NLINK_PARSER_MESSAGE_LINKTRACKNODE0_H