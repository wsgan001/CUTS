/**
 * @file 			JbiClient.java
 * 
 * $Id$
 * 
 * @author 			James H. Hill
 */

package cuts.java.jbi.client;
import mil.af.rl.im.capi.client.impl.ConnectionManager;
import org.infospherics.jbi.client.Connection;
import org.infospherics.jbi.client.exception.*;
import java.util.Timer;
import java.util.ArrayList;

/**
 * @class JbiClient
 * 
 * Base class that all JbiClients in CUTS inherit. It defines the 
 * required interface necessary for integrating with the CUTS testing
 * environment.
 */
public abstract class JbiClient implements Runnable
{
	/// Single connection manager owned by the client.
	private static ConnectionManager jbiConnMgr = new ConnectionManager();

	/// The default connection for the client.
	private Connection jbiConn = null;
	
  /// The timer for the client's periodic task.
  private Timer timer_ = new Timer ();
  
	/// Address of the server.
	private String serverAddress;
	
	/// Username for the client.
	private String username;
	
	/// Password for the client.
	private String password;
	
	/// The client has begun the shutdown process.
	private boolean shutdown_ = false;
	
  /**
   * Collection of sinks for the client.
   */
  private ArrayList <JbiSink> sinks_ = new ArrayList <JbiSink> ();
  
  /**
   * Collection of sources for the client.
   */
  private ArrayList <JbiSource> sources_ = new ArrayList <JbiSource> ();
  
	public abstract void init ()
    throws PermissionDeniedException, UnsupportedVersionException;
	
	public abstract void activate ();
	
	public abstract void deactivate ();
	
	public abstract void fini ();
	
  /**
   * Get the contained JBI connection. This is accessible only to 
   * the subclasses of the client.
   * 
   * @return        The JBI connection for the client.
   */
  public Connection getJbiConnection ()
  {
    return this.jbiConn;
  }
  
  /**
   * Parse the command-line arguments for the client.
   * 
   * @param         args            The command-line arguments.
   * @retval        0               Successfully parsed arguments.
   * @retval        -1              Failed to parse command-line arguments.
   */
  public int parseArgs (String [] args)
  {
    switch (args.length)
    {
    case 3:
      // Set the user credentials.
      this.setUserCredentials (args[1], args[2]);
      
    case 1:
      // Set the server address.
      this.setServerAddress (args[0]);
      break;
      
    default:
      // Print an error message, and the usage.
      System.err.println ("*** error: expected 1 or 3 arguments; got " + 
                          args.length + " arguments");
      System.err.println ("USAGE: java [OPTIONS] <jbiclient> serveraddr [username password]");
      break;
    }
    
    return 0;
  }
  
	/**
	 * 
	 */
	public void setServerAddress (String addr)
	{
		this.serverAddress = addr;
	}
	
	/**
	 * 
	 */
	public void setUserCredentials (String user, String passwd)
	{
		this.username = user;
		this.password = passwd;
	}
	
  /**
   * Get the timer for the client. This allows the subclass to 
   * add new periodic tasks to the timer. 
   */
  protected Timer getTimer ()
  {
    return this.timer_;
  }
  
  protected void registerInfoSequence (JbiSource source)
  {
    this.sources_.add (source);
  }
  
  protected void registerInfoSequence (JbiSink sink)
  {
    this.sinks_.add (sink);
  }
  
	/**
	 * Main entry point for the JBI client. This will execute the 
	 * test sequence for the client.
	 */
	public void run ()
	{
	  // Register the shutdown hook for the client. This will 
	  // ensure the client releases all it's resources.
  	Runtime.getRuntime ().addShutdownHook (new ShutdownThread (this));
  	
		try
		{
			// Create the default connection for the client. Before we 
			// can connect, we have to authenticate the client with the
			// server.
			this.jbiConn = 
				this.jbiConnMgr.createConnection (
						"connection:" + this.serverAddress,
						this.createConnectionDescriptor (this.serverAddress)); 

			this.jbiConn.authenticate (
					this.createUserCredentials(this.username, this.password));
			
			this.jbiConn.connect ();
			
			// Initialize the application. This is where the actual
			// client will create all its publisher/subscriber sequences.
			this.init ();

      // Activate all the sinks in this client.
      for (JbiSink sink : this.sinks_)
        sink.open ();
      
      // Activate all the sources in this client.
      for (JbiSource source : this.sources_)
        source.open ();
      
			// Signal the client to activate itself.
			this.activate ();
		}
    catch (Exception e)
    {
      e.printStackTrace ();
    }
	}
	
	/**
	 * Shutdown the client. This will cause the client to go through
	 * its deactivate
	 */
	public void shutdown ()
	{
	  // Make sure we are not already performing a shutdown.
	  if (this.shutdown_)
	    return;
	  
	  this.shutdown_ = true;
	  
	  try 
	  {
      // Cancel all the periodic tasks.
      this.timer_.cancel ();
      
      // Signal the client to deactivate itself.
			this.deactivate ();		

      // Deactivate all the sources in this client.
      for (JbiSource source : this.sources_)
        source.close ();
      
      // Deactivate all the sinks in this client.
      for (JbiSink sink : this.sinks_)
        sink.close ();
       
      // This is where the client will destroy all its publisher/
			// subscriber sequences.
			this.fini ();
	  }
	  catch (Exception e)
	  {
	    e.printStackTrace ();
	  }
		finally
		{
			// Close the connection and destroy it.
      try
      {
  			this.jbiConn.disconnect();			
  			this.jbiConnMgr.destroyConnection (this.jbiConn);
  
  			// Reset the connection just in case. :)
  			this.jbiConn = null;
      }
      catch (Exception e)
      {
        e.printStackTrace ();
      }
		}
	}
	
  /**
   * Helper method for generating the user credentials. Given the
   * \a username and \a password, it will return a valid XML document,
   * or string, that can be passed to the Apollo server for validation.
   * 
   * @param         username          Username to validate
   * @param         password          Password associated with \a username
   * @return        XML descriptor of the user credentials
   */
	private static String createUserCredentials (String username, String password) 
	{
		return new String("<credentials>" + 
				                "<username>"+ username + "</username>" +
				                "<password>"+ password +"</password>" +
				              "</credentials>");
	}
	
  /**
   * Helper methods to create a connection deescriptor. Given the
   * \a ipAddress of the server, this method will return a valid XML
   * document, or string, that can be used to create an Apollo 
   * connection.
   * 
   * @param         ipAddress         Ip address of the Apollo server.
   * @return        XML descriptor of the connection.
   */
	private static String createConnectionDescriptor (String ipAddress)
	{
		return new String ("<ConnectionDescriptor>" + 
			                   "<PlatformDescriptor>" +
			                     "<platformIP>" + ipAddress + "</platformIP>" +
			                   "</PlatformDescriptor>" +
				               "</ConnectionDescriptor>");
	}
	
	/**
	 * Thread that is responsible for handling the shutdown of the 
	 * JbiClient. This class is registered as a shutdown hook for
	 * with the virtual machine. Whenever the user presses Ctrl + C
	 * the run () method is invoked.
	 */
	class ShutdownThread extends Thread
	{
	  /// Target JBI client.
	  private JbiClient jbiClient_ = null;
	  
	  /**
	   * Initializing constructor.
	   *
	   * @param[in]       client        The target client.
	   */
	  public ShutdownThread (JbiClient client)
	  {
	    this.jbiClient_ = client;
	  }
	  
	  /**
	   * Callback method for the thread routine.
	   */
	  public void run ()
	  {
	    this.jbiClient_.shutdown ();
	  }
	}
}

