// -*- C++ -*-

//=============================================================================
/**
 *  @file         OpenSplice.h
 *
 *  $Id$
 *
 *  @author       James H. Hill
 */
//=============================================================================

#ifndef _ICCM_OPENSPLICE_H_
#define _ICCM_OPENSPLICE_H_

// QoS define guards for non-standard features
#define ICCM_DDS_SUPPORTS_SCHEDULING_QOS
#define ICCM_DDS_SUPPORTS_SYNCHRONOUS_RELIABILITY_QOS
#define ICCM_DDS_SUPPORTS_SHARE_QOS
#define ICCM_DDS_SUPPORTS_READER_INVALID_SAMPLES
#define ICCM_DDS_SUPPORTS_SUBSCRIPTION_KEY_QOS
#define ICCM_DDS_SUPPORTS_READER_LIFESPAN_QOS
#define ICCM_DDS_SUPPORTS_WRITER_AUTOPURGE_SUSPENDED_SAMPLES
#define ICCM_DDS_SUPPORTS_WRITER_AUTOUNREGISTER

#include "cuts/iccm/arch/dds/servant/DDS_EventConsumer.h"
#include "cuts/iccm/arch/dds/servant/DDS_Publisher_Table.h"
#include "cuts/iccm/arch/dds/servant/DDS_Publisher.h"
#include "cuts/iccm/arch/dds/servant/DDS_Servant.h"
#include "cuts/iccm/arch/dds/servant/DDS_Traits_T.h"

#include "OpenSplice_EventsS.h"

#include "OpenSplice_svnt_export.h"

#include "ccpp_dds_dcps.h"

namespace iCCM
{
// Forward decl.
class OpenSplice;

// Forward decl.
template <typename EVENT>
class OpenSplice_Publisher_T;

template <typename EVENT>
class OpenSplice_Publisher_Table_T;

template <typename SERVANT, typename EVENT>
class OpenSplice_EventConsumer_T;

class OpenSplice_Servant;

// Type definition.
typedef DDS_Publisher_Table <OpenSplice> OpenSplice_Publisher_Table;
typedef DDS_Publisher <OpenSplice> OpenSplice_Publisher;
typedef DDS_EventConsumer <OpenSplice> OpenSplice_EventConsumer;

/**
 * @class OpenSplice
 */
class ICCM_OPENSPLICE_SVNT_Export OpenSplice
{
public:
  template <typename EVENT>
  struct event_traits_type
  {
    typedef DDS_Traits_T <EVENT> result_type;
  };

  template <typename EVENT>
  struct typed_publisher_type
  {
    typedef OpenSplice_Publisher_T <EVENT> result_type;
  };

  template <typename EVENT>
  struct typed_publisher_table_type
  {
    typedef OpenSplice_Publisher_Table_T <EVENT> result_type;
  };

  template <typename SERVANT, typename EVENT>
  struct typed_eventconsumer_type
  {
    typedef OpenSplice_EventConsumer_T <SERVANT, EVENT> result_type;
  };

  typedef OpenSplice_EventConsumer eventconsumer_type;
  typedef OpenSplice_Publisher publisher_type;
  typedef OpenSplice_Publisher_Table publisher_table_type;
  typedef OpenSplice_Servant servant_type;

  typedef ::Components::OpenSplice::EventConsumer corba_eventconsumer_type;
  typedef ::Components::OpenSplice::EventConsumer_var corba_eventconsumer_var_type;
  typedef ::POA_Components::OpenSplice::EventConsumer corba_poa_eventconsumer_type;

  typedef ::DDS::DomainParticipantFactory_var domainparticipantfactory_var_type;
  typedef ::DDS::DomainParticipantFactory_ptr domainparticipantfactory_ptr_type;

  typedef ::DDS::DomainParticipant_var domainparticipant_var_type;
  typedef ::DDS::DomainParticipant_ptr domainparticipant_ptr_type;

  typedef ::DDS::TopicQos topicqos_type;
  typedef ::DDS::DomainParticipantQos domainparticipantqos_type;
  typedef ::DDS::DataReaderQos datareaderqos_type;
  typedef ::DDS::DataWriterQos datawriterqos_type;
  typedef ::DDS::PublisherQos publisherqos_type;
  typedef ::DDS::SubscriberQos subscriberqos_type;

  typedef ::DDS::Topic_ptr topic_ptr_type;
  typedef ::DDS::Topic_var topic_var_type;

  typedef ::DDS::Publisher_ptr publisher_ptr_type;
  typedef ::DDS::Publisher_var publisher_var_type;

  typedef ::DDS::Subscriber_ptr subscriber_ptr_type;
  typedef ::DDS::Subscriber_var subscriber_var_type;

  typedef ::DDS::DataWriter_ptr datawriter_ptr_type;
  typedef ::DDS::DataWriter_var datawriter_var_type;

  typedef ::DDS::DataReader_ptr datareader_ptr_type;
  typedef ::DDS::DataReader_var datareader_var_type;

  typedef ::DDS::DataReaderListener datareaderlistener_type;
  typedef ::DDS::SampleInfoSeq sampleinfoseq_type;

  typedef ::DDS::ReturnCode_t returncode_type;
  typedef ::DDS::InstanceHandle_t instancehandle_type;

  typedef ::DDS::RequestedDeadlineMissedStatus requesteddeadlinemissedstatus_type;
  typedef ::DDS::RequestedIncompatibleQosStatus requestedincompatibleqosstatus_type;
  typedef ::DDS::SampleRejectedStatus samplerejectedstatus_type;
  typedef ::DDS::LivelinessChangedStatus livelinesschangedstatus_type;
  typedef ::DDS::SubscriptionMatchedStatus subscriptionmatchedstatus_type;
  typedef ::DDS::SampleLostStatus sampleloststatus_type;

  typedef ::DDS::StatusKind statusmask_type;
  static const statusmask_type STATUS_MASK_NONE = ::DDS::STATUS_MASK_NONE;
  static const statusmask_type STATUS_MASK_DATA_AVAILABLE = ::DDS::DATA_AVAILABLE_STATUS;

  static const unsigned long ANY_SAMPLE_STATE = ::DDS::ANY_SAMPLE_STATE;
  static const unsigned long ANY_VIEW_STATE = ::DDS::ANY_VIEW_STATE;
  static const unsigned long ANY_INSTANCE_STATE = ::DDS::ANY_INSTANCE_STATE;

  static const instancehandle_type HANDLE_NIL = ::DDS::HANDLE_NIL;

  static const returncode_type RETCODE_OK = ::DDS::RETCODE_OK;
  static const returncode_type RETCODE_NO_DATA = ::DDS::RETCODE_NO_DATA;

  static const long LENGTH_UNLIMITED = ::DDS::LENGTH_UNLIMITED;

  static domainparticipantfactory_ptr_type get_domainparticipantfactory_instance (void);

  static const topicqos_type * topic_qos_default (void);
  static const domainparticipantqos_type & domainparticipant_qos_default (void);
  static const subscriberqos_type & subscriber_qos_default (void);
  static const publisherqos_type & publisher_qos_default (void);
  static const datawriterqos_type & datawriter_qos_default (void);
  static const datareaderqos_type * datareader_qos_default (void);

  // Qos typedefs
  enum SchedulingClassQosPolicyKind
  {
    SCHEDULE_DEFAULT = ::DDS::SCHEDULE_DEFAULT,
    SCHEDULE_TIMESHARING = ::DDS::SCHEDULE_TIMESHARING,
    SCHEDULE_REALTIME = ::DDS::SCHEDULE_REALTIME
  };
  typedef ::DDS::SchedulingClassQosPolicyKind scheduling_class_kind_type;
  typedef ::DDS::SchedulingClassQosPolicy scheduling_class_type;

  enum SchedulingPriorityQosPolicyKind
  {
    PRIORITY_RELATIVE = ::DDS::PRIORITY_RELATIVE,
    PRIORITY_ABSOLUTE = ::DDS::PRIORITY_ABSOLUTE
  };
  typedef ::DDS::SchedulingPriorityQosPolicyKind scheduling_priority_kind_type;
  typedef ::DDS::SchedulingPriorityQosPolicy scheduling_priority_type;

  typedef ::DDS::SchedulingQosPolicy scheduling_type;
  typedef ::DDS::ShareQosPolicy share_type;
  typedef ::DDS::SubscriptionKeyQosPolicy subscription_key_type;
  typedef ::DDS::ReaderLifespanQosPolicy reader_lifespan_type;

  typedef ::DDS::Duration_t duration_type;

  enum HistoryQosPolicyKind
  {
    KEEP_ALL_HISTORY_QOS = ::DDS::KEEP_ALL_HISTORY_QOS,
    KEEP_LAST_HISTORY_QOS = ::DDS::KEEP_LAST_HISTORY_QOS
  };
  typedef ::DDS::HistoryQosPolicyKind history_kind_type;

  enum LivelinessQosPolicyKind
  {
    AUTOMATIC_LIVELINESS_QOS = ::DDS::AUTOMATIC_LIVELINESS_QOS,
    MANUAL_BY_PARTICIPANT_LIVELINESS_QOS = ::DDS::MANUAL_BY_PARTICIPANT_LIVELINESS_QOS,
    MANUAL_BY_TOPIC_LIVELINESS_QOS = ::DDS::MANUAL_BY_TOPIC_LIVELINESS_QOS
  };
  typedef ::DDS::LivelinessQosPolicyKind liveliness_kind_type;

  enum DurabilityQosPolicyKind
  {
    PERSISTENT_DURABILITY_QOS = ::DDS::PERSISTENT_DURABILITY_QOS,
    TRANSIENT_DURABILITY_QOS = ::DDS::TRANSIENT_DURABILITY_QOS,
    TRANSIENT_LOCAL_DURABILITY_QOS = ::DDS::TRANSIENT_LOCAL_DURABILITY_QOS,
    VOLATILE_DURABILITY_QOS = ::DDS::VOLATILE_DURABILITY_QOS
  };
  typedef ::DDS::DurabilityQosPolicyKind durability_kind_type;

  enum ReliabilityQosPolicyKind
  {
    BEST_EFFORT_RELIABILITY_QOS = ::DDS::BEST_EFFORT_RELIABILITY_QOS,
    RELIABLE_RELIABILITY_QOS = ::DDS::RELIABLE_RELIABILITY_QOS
  };
  typedef ::DDS::ReliabilityQosPolicyKind reliability_kind_type;

  enum DestinationOrderQosPolicyKind
  {
    BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS = ::DDS::BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS,
    BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS = ::DDS::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS
  };
  typedef ::DDS::DestinationOrderQosPolicyKind destination_order_kind_type;

  enum OwnershipQosPolicyKind
  {
    EXCLUSIVE_OWNERSHIP_QOS = ::DDS::EXCLUSIVE_OWNERSHIP_QOS,
    SHARED_OWNERSHIP_QOS = ::DDS::SHARED_OWNERSHIP_QOS
  };
  typedef ::DDS::OwnershipQosPolicyKind ownership_kind_type;

  enum PresentationQosPolicyAccessScopeKind
  {
    GROUP_PRESENTATION_QOS = ::DDS::GROUP_PRESENTATION_QOS,
    INSTANCE_PRESENTATION_QOS = ::DDS::INSTANCE_PRESENTATION_QOS,
    TOPIC_PRESENTATION_QOS = ::DDS::TOPIC_PRESENTATION_QOS
  };
  typedef ::DDS::PresentationQosPolicyAccessScopeKind access_scope_type;

  typedef ::DDS::DurabilityQosPolicy durability_type;
  typedef ::DDS::DurabilityServiceQosPolicy durability_service_type;
  typedef ::DDS::DeadlineQosPolicy deadline_type;
  typedef ::DDS::LatencyBudgetQosPolicy latency_budget_type;
  typedef ::DDS::LivelinessQosPolicy liveliness_type;
  typedef ::DDS::ReliabilityQosPolicy reliability_type;
  typedef ::DDS::DestinationOrderQosPolicy destination_order_type;
  typedef ::DDS::HistoryQosPolicy history_type;
  typedef ::DDS::ResourceLimitsQosPolicy resource_limits_type;
  typedef ::DDS::TransportPriorityQosPolicy transport_priority_type;
  typedef ::DDS::LifespanQosPolicy lifespan_type;
  typedef ::DDS::OwnershipQosPolicy ownership_type;
  typedef ::DDS::PresentationQosPolicy presentation_type;
  typedef ::DDS::StringSeq stringseq_type;
  static void copy_string (stringseq_type & dst, const ::iccm::StringSeq & src);
  typedef ::DDS::PartitionQosPolicy partition_type;
  typedef ::DDS::GroupDataQosPolicy group_data_type;
  typedef ::DDS::UserDataQosPolicy user_data_type;
  typedef ::DDS::TimeBasedFilterQosPolicy time_based_filter_type;
  typedef ::DDS::ReaderDataLifecycleQosPolicy reader_data_lifecycle_type;
  typedef ::DDS::OwnershipStrengthQosPolicy ownership_strength_type;
  typedef ::DDS::WriterDataLifecycleQosPolicy writer_data_lifecycle_type;
  typedef ::DDS::EntityFactoryQosPolicy entity_factory_type;

  template <typename TYPE_SUPPORT>
  static returncode_type register_type (domainparticipant_ptr_type p, ACE_CString & type_name)
  {
    TYPE_SUPPORT * type_support = 0;

    ACE_NEW_THROW_EX (type_support,
                      TYPE_SUPPORT (),
                      ::CORBA::NO_MEMORY ());

    ACE_Auto_Ptr <TYPE_SUPPORT> auto_clean (type_support);
    type_name = type_support->get_type_name ();
    return type_support->register_type (p, type_name.c_str ());
  }

  template <typename T>
  static bool _is_nil (T val)
  {
    return ::CORBA::is_nil (val.in ());
  }

  template <typename T>
  static bool _is_nil (T * val)
  {
    return ::CORBA::is_nil (val);
  }

  template <typename T>
  static void _add_ref (T val)
  {
    val->_add_ref ();
  }

  template <typename T>
  static typename T::_retn_type _duplicate (T val)
  {
    return T::_obj_type::_duplicate (val.in ());
  }

  template <typename T>
  static T * _duplicate (T * val)
  {
    return T::_duplicate (val);
  }

  template <typename T1, typename T2>
  static typename T1::_ptr_type _writer_cast (T2 val)
  {
    return T1::_narrow (val);
  }

  template <typename T1, typename T2>
  static typename T1::_ptr_type _reader_cast (T2 val)
  {
    return T1::_narrow (val);
  }

  template <typename T>
  static typename T::_retn_type _retn (T val)
  {
    return val._retn ();
  }
};

};

#if defined (__CUTS_INLINE__)
#include "OpenSplice.inl"
#endif

#endif  // !define _ICCM_OPENSPLICE_H_
