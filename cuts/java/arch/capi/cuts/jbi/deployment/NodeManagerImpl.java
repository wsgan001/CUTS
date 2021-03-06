//  -*- java -*-

//=============================================================================
/**
 * @file          NodeManagerImpl.java
 *
 * $Id$
 *
 * @author        James H. Hill
 */
//=============================================================================

package cuts.jbi.deployment;
import org.omg.PortableServer.*;
import org.apache.log4j.Logger;
import java.util.*;

/**
 * @class JbiNodeManager
 *
 * Default implemenation of the NodeManager interface. This class also
 * serves as the main entry point for the node manager in the JBI deployment
 * framework.
 */
public class NodeManagerImpl
  extends NodeManagerPOA
{
  /// The ORB assigned to the node manager.
  private org.omg.CORBA.ORB orb_ = null;

  /// Collection of node application managers.
  private final HashMap<
    NodeApplicationManager, NodeApplicationManagerImpl> managers_ =
    new HashMap <NodeApplicationManager, NodeApplicationManagerImpl> ();

  private String alias_ = null;

  private final Logger logger_ =
    Logger.getLogger (NodeManagerImpl.class);

  /**
   * Default constructor.
   */
  public NodeManagerImpl (org.omg.CORBA.ORB orb, String name)
  {
    this.orb_ = orb;
    this.alias_ = name;

    if (this.alias_ != null)
      this.logger_.debug ("alias name is " + this.alias_);
    else
      this.logger_.debug ("alias name is not defined");
  }

  /**
   * Prepare the deployment plan for deployment (or realization). This will
   * create a new node application manager for the plan, and return it
   * to the execution manager.
   */
  public NodeApplicationManager preparePlan (DeploymentPlan plan)
  {
    NodeApplicationManager manager = null;
    this.logger_.debug ("preparing plan " + plan.UUID + " for deployment");

    try
    {
      // Create a new node application manager.
      NodeApplicationManagerImpl namImpl =
        new NodeApplicationManagerImpl (this.orb_);

      // Activate the node application manager.
      this.logger_.debug ("activating a new node application manager");

      org.omg.PortableServer.POA poa =
        org.omg.PortableServer.POAHelper.narrow (
        this.orb_.resolve_initial_references ("RootPOA"));

      manager =
        NodeApplicationManagerHelper.narrow (
        poa.servant_to_reference (namImpl));

      // Let the node application manager know of the process groups
      // that it will need spawn, and the instances it will need to
      // install in the spawned process.
      for (ComponentInstanceDescriptor cid : plan.componentInstances)
      {
        if (this.alias_.equals (cid.targetHost))
          namImpl.prepareInstance (cid.processGroup, cid.instanceName);
      }

      // Save the manager with it's implementation.
      this.managers_.put (manager, namImpl);
    }
    catch (Exception ex)
    {
      this.logger_.debug (ex.getMessage (), ex);
    }
    finally
    {
      // Return the node application manager to the client, which is
      // the execution manager in this case.
      return manager;
    }
  }

  /**
   * Destroy the specified manager. This will force the manager to
   * release all its resources, which includes destoying all
   * application and their instances.
   */
  public void destroyManager (NodeApplicationManager manager)
  {
    // Get the actual implementation of the CORBA object.
    this.logger_.debug ("locating node application manager's implementation");
    NodeApplicationManagerImpl namImpl = this.managers_.get (manager);

    if (namImpl != null)
    {
      // Remove the node application manager.
      this.logger_.debug ("removing node application manager");
      this.managers_.remove (manager);
    }
  }
}
