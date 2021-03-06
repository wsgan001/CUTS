#!/bin/bash

################################################################################
##
## @file            SingleProcess.sh
##
## $Id$
##
## @author          James H. Hill
##
## Helper script for manually deploying multiple JBI client objects into
## a single application process.
##
################################################################################

# common variables
JAVA_CMD=java

CUTS_CLASSPATH=${CUTS_ROOT}/lib/cuts.java.capi.deployment.jar:${CUTS_ROOT}/lib/cuts.java.jar:${CUTS_ROOT}/lib/cuts.java.capi.jar:${CUTS_ROOT}/contrib/java/spring.jar:${CUTS_ROOT}/contrib/java/log4j-1.2.15.jar:${CUTS_ROOT}/contrib/java/commons-logging-1.1.1.jar:${CUTS_ROOT}/contrib/java/castor-1.2.jar:${CUTS_ROOT}/contrib/java/jacorb.jar:${CUTS_ROOT}/contrib/java/avalon-framework-4.1.5.jar:${CUTS_ROOT}/contrib/java/logkit-1.2.jar

JBI_CLASSPATH=${JBI_ROOT}/lib/capi1.5.jar:${JBI_ROOT}/lib/dom4j-1.6.1.jar:${JBI_ROOT}/lib/jaxen-1.1.1.jar:${JBI_ROOT}/lib/jbossall-client.jar:${JBI_ROOT}/lib/xpp3_xpath-1.1.4c.jar:${JBI_ROOT}/lib/xpp3-1.1.4c.jar

# execute the application
exec ${JAVA_CMD} ${JAVA_OPTS}                                       \
    -Dorg.omg.CORBA.ORBClass=org.jacorb.orb.ORB                     \
    -Dorg.omg.CORBA.ORBSingletonClass=org.jacorb.orb.ORBSingleton   \
    -classpath ${CUTS_CLASSPATH}:${JBI_CLASSPATH}:${CLASSPATH}      \
    cuts.jbi.deployment.JbiClientApp "$@"
