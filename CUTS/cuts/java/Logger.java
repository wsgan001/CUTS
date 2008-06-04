/**
 * @file        Logger.java
 * 
 * $Id$
 * 
 * @author      James H. Hill
 */

package cuts.java;

//import cuts.java.LogPriority;

/**
 * Basic logger class for integrating with ACE logging framework. This
 * class define the minimum methods needed to interface with ACE. 
 * Currently, there is only one log (...) method, which simplifies and 
 * minimizes the amount to data/variables that must be converted from
 * managed to unmanaged code. 
 */
public class Logger
{
  /**
   * Default constructor.
   */
  public Logger () 
  {
    
  }
  
  /**
   * Native function that calls the ACE_Log_Msg::log (...). The 
   * implementation of this function using the logging singleton provided
   * by ACE. It is also possible to use some formating options, such as 
   * thread/process id and timestamp, which will be inserted into the 
   * final log message generated by the client. 
   * 
   * The following is list of valid options when proceeded by an percent
   * sign (%):
   * 
   * 'M': print the name of the message priority
   * 'P': print the current process id
   * 'T': print timestamp in hour:minute:sec:usec format
   * 'D': print timestamp as Weekday Month Day Year hour:minute:sec.usec
   * 't': print thread id (1 if single-threaded)
   * 'I': indent according to nesting depth
   *     
   * @param[in]         priority        Priority of the message.
   * @param[in]         message         The actual log message.
   */
  public native void log (int priority, String message);
  
  static {
    System.loadLibrary ("cuts.java");
  }
}
