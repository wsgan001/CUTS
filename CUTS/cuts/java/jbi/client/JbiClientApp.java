/**
 * @file        JbiClientApp.java
 *
 * $Id$
 *
 * @author      James H. Hill
 */

package cuts.java.jbi.client;

import mil.af.rl.im.capi.client.impl.ConnectionManager;
import org.springframework.beans.factory.BeanFactory;
import org.springframework.context.support.FileSystemXmlApplicationContext;

/**
 * @class JbiClientApp
 */
public class JbiClientApp
{
  private static ConnectionManager connManager_ = new ConnectionManager ();

  private BeanFactory beanFactory_;
  
  private String beanName_;
  
  public JbiClientApp ()
  {
  
  }
  
  public void run (String [] args)
  {
    // Parse the command-line arguments.
    this.parseArgs (args);
    
    // Load the JBI client using the factory.
    JbiClient client = 
      (JbiClient) this.beanFactory_.getBean (this.beanName_);

    if (this.connManager_ == null)
	System.err.println ("Wow, this is NULL");

    client.setJbiConnectionManager (this.connManager_);

    // Register the shutdown hook for the client. This will 
    // ensure the client releases all it's resources.
    Runtime.getRuntime ().
      addShutdownHook (new JbiClientAppShutdownThread (client));  	

    // Run the client.
    client.run ();
  }
  
  /**
   * Parse the command-line arguments. This method needs to be 
   * enhanced to do more complex command-line argument parsing.
   *
   * @param[in]       args        The command-line arguments.
   */
  private void parseArgs (String [] args)
  {
    // Save the name of the target bean.
    this.beanName_ = args[0];
    
    // Convert the name of the bean to its location on disk.
    String beanFile = this.beanName_.replace ('.', '/');
    beanFile += ".qic";
    
    // Load the factory for the specified bean.
    this.beanFactory_ = new FileSystemXmlApplicationContext (beanFile);
  }
  
  /**
   * Main entry point for the application.
   *
   * @param[in]     args        Command-line arguments.
   */
  public static void main (String [] args)
  {
    try
    {
      JbiClientApp jbiClientApp = new JbiClientApp ();
      jbiClientApp.run (args);
    }
    catch (Exception e)
    {
      e.printStackTrace ();
    }
  }
}
