// $Id$

#include "Response_Time_Evaluator.h"

#if !defined (__CUTS_INLINE__)
#include "Response_Time_Evaluator.inl"
#endif

#include "Propagate_Arrival_Rates.h"
#include "Component_Assembly.h"

//
// evaluate
//
void CUTS_Response_Time_Evaluator::
evaluate (CUTS_Component_Assembly & assembly)
{
  // 1. Propogate the arrival rates throughout the assembly.
  CUTS_Propagate_Arrival_Rates arrivals;
  assembly.accept (arrivals);

  // 2. Update service times based on component structure.

  // 3. Update service times based on component deployment.
}