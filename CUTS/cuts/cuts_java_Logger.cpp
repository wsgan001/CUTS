// $Id$

#include "cuts_java_Logger.h"
#include "ace/Log_Msg.h"

//
// get_LogPriority_value
//
static int get_LogPriority_value (JNIEnv * env, jobject priority)
{
  // Get the class for the object.
  jclass cls = env->GetObjectClass (priority);

  // Get the method for the object (i.e., value ()").
  jmethodID mid = env->GetMethodID (cls, "value", "()I");
  
  // Invoke the method to retrieve the value.
  if (mid != 0)
    return env->CallIntMethod (priority, mid);

  return -1;
}

//
// Java_cuts_java_Logger_log
//
void JNICALL 
Java_cuts_java_Logger_log (JNIEnv * env, 
                           jobject object, 
                           jobject priority, 
                           jstring message)
{
  // Extract the value of the priority. We need to convert
  // it into an ACE_Log_Priority value.
  ACE_Log_Priority level = 
    static_cast <ACE_Log_Priority> (get_LogPriority_value (env, priority));

  if (level != -1)
  {
    // Extract the string message from the Java environment. We
    // are not going to changes its formatting.
    const char * msg = env->GetStringUTFChars (message, 0);

    if (msg != 0)
    {
      // Generate the log message. Should we use the macro, i.e., 
      // ACE_DEBUG or ACE_ERROR, or directly use ACE_LOG_MSG??
      ACE_LOG_MSG->log (level, msg);

      // Release the extracted string.
      env->ReleaseStringUTFChars (message, msg);
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
                  "*** error: failed to extract log message\n"));
    }
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
                "*** error: failed to extract priority value\n"));
  }
}
