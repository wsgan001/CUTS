#include "ccm/CCM_StandardConfiguratorC.h"
namespace iCCM
{
  template <typename SERVANT, typename EVENT>
  TAO_EventConsumer_Task_T<SERVANT, EVENT>::TAO_EventConsumer_Task_T (SERVANT * servant, CALLBACK_METHOD callback)
    : servant_ (servant),
      callback_ (callback)
  {
  }

  template <typename SERVANT, typename EVENT>
  int TAO_EventConsumer_Task_T<SERVANT, EVENT>::svc (void)
  {
    this->set_affinity ();

    EVENT * ev;
    // Main loop
    while (this->getq (ev) != -1)
    {
      if (0 != this->servant_)
      {
        (*this->servant_.*this->callback_) (ev);
        CORBA::remove_ref (ev);
      }
    }

    return 0;
  }
}
