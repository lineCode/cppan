#if !defined(CPPAN_SUPPORT_BOOST_SERIALIZATION_INCLUDED)
#define CPPAN_SUPPORT_BOOST_SERIALIZATION_INCLUDED

#include <cppan/define_member_detector.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

namespace cppan { 
namespace detail {

template<typename Archive>
struct serialize_visitor
{
    Archive& ar_;
    
    serialize_visitor(Archive& ar, const unsigned int) : ar_(ar) {} 

    template<typename T>
    void operator()(T& member) const
    {
        ar_ & member.value_;
    }
};
}

#if !defined(CPPAN_DOXYGEN)
CPPAN_DEFINE_MEMBER_DETECTOR(no_serialization);

#if defined(CPPAN_ENABLE_BOOST_SERIALIZATION)
#  undef CPPAN_ENABLE_BOOST_SERIALIZATION
#endif

#define CPPAN_ENABLE_BOOST_SERIALIZATION \
    friend class boost::serialization::access; \
    template<typename Archive> \
    void serialize(Archive& ar, const unsigned int version) \
    { \
        ::cppan::serialize(ar, *this, version); \
    }

#endif

// TODO: Add support for class version
template<typename Archive, typename AnnotatedTuple>
void serialize(Archive& ar, AnnotatedTuple& t, const unsigned int version)
{
    using namespace boost::fusion;
    using namespace cppan::detail;
    using boost::mpl::_;
    using boost::mpl::not_;

    return for_each(
        filter_view<AnnotatedTuple, not_< has_no_serialization<_> > >(t)
      , serialize_visitor<Archive>(ar, version)
      );
}

}

#endif
