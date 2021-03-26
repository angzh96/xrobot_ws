// Generated by gencpp from file nlink_parser/TofsenseCascade.msg
// DO NOT EDIT!


#ifndef NLINK_PARSER_MESSAGE_TOFSENSECASCADE_H
#define NLINK_PARSER_MESSAGE_TOFSENSECASCADE_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <nlink_parser/TofsenseFrame0.h>

namespace nlink_parser
{
template <class ContainerAllocator>
struct TofsenseCascade_
{
  typedef TofsenseCascade_<ContainerAllocator> Type;

  TofsenseCascade_()
    : nodes()  {
    }
  TofsenseCascade_(const ContainerAllocator& _alloc)
    : nodes(_alloc)  {
  (void)_alloc;
    }



   typedef std::vector< ::nlink_parser::TofsenseFrame0_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::nlink_parser::TofsenseFrame0_<ContainerAllocator> >::other >  _nodes_type;
  _nodes_type nodes;




  typedef boost::shared_ptr< ::nlink_parser::TofsenseCascade_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::nlink_parser::TofsenseCascade_<ContainerAllocator> const> ConstPtr;

}; // struct TofsenseCascade_

typedef ::nlink_parser::TofsenseCascade_<std::allocator<void> > TofsenseCascade;

typedef boost::shared_ptr< ::nlink_parser::TofsenseCascade > TofsenseCascadePtr;
typedef boost::shared_ptr< ::nlink_parser::TofsenseCascade const> TofsenseCascadeConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::nlink_parser::TofsenseCascade_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::nlink_parser::TofsenseCascade_<ContainerAllocator> >::stream(s, "", v);
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
struct IsFixedSize< ::nlink_parser::TofsenseCascade_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::nlink_parser::TofsenseCascade_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::nlink_parser::TofsenseCascade_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::nlink_parser::TofsenseCascade_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::nlink_parser::TofsenseCascade_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::nlink_parser::TofsenseCascade_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::nlink_parser::TofsenseCascade_<ContainerAllocator> >
{
  static const char* value()
  {
    return "6f5a4318b44b44ec8258733a82bf1f6c";
  }

  static const char* value(const ::nlink_parser::TofsenseCascade_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x6f5a4318b44b44ecULL;
  static const uint64_t static_value2 = 0x8258733a82bf1f6cULL;
};

template<class ContainerAllocator>
struct DataType< ::nlink_parser::TofsenseCascade_<ContainerAllocator> >
{
  static const char* value()
  {
    return "nlink_parser/TofsenseCascade";
  }

  static const char* value(const ::nlink_parser::TofsenseCascade_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::nlink_parser::TofsenseCascade_<ContainerAllocator> >
{
  static const char* value()
  {
    return "TofsenseFrame0[] nodes\n\
\n\
================================================================================\n\
MSG: nlink_parser/TofsenseFrame0\n\
uint8 id\n\
uint32 system_time\n\
float32 dis\n\
uint8 dis_status\n\
uint16 signal_strength\n\
";
  }

  static const char* value(const ::nlink_parser::TofsenseCascade_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::nlink_parser::TofsenseCascade_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.nodes);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct TofsenseCascade_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::nlink_parser::TofsenseCascade_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::nlink_parser::TofsenseCascade_<ContainerAllocator>& v)
  {
    s << indent << "nodes[]" << std::endl;
    for (size_t i = 0; i < v.nodes.size(); ++i)
    {
      s << indent << "  nodes[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::nlink_parser::TofsenseFrame0_<ContainerAllocator> >::stream(s, indent + "    ", v.nodes[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // NLINK_PARSER_MESSAGE_TOFSENSECASCADE_H
