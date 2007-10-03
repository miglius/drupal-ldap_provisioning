<?php
/* $Id$ */

require_once(drupal_get_path('module', 'ldapauth') .'/ldap_integration/LDAPInterface.php');

class LDAPInterface2 extends LDAPInterface {

  function create_entry($dn, $attributes) {
    set_error_handler(array('LDAPInterface', 'void_error_handler'));
    $ret = ldap_add($this->connection, $dn, $attributes);
    restore_error_handler();

    return $ret;
  }

  function rename_entry($dn, $newrdn, $newparent, $deleteoldrdn) {
    set_error_handler(array('LDAPInterface', 'void_error_handler'));
    $ret = ldap_rename($this->connection, $dn, $newrdn, $newparent, $deleteoldrdn);
    restore_error_handler();

    return $ret;
  }

  function delete_entry($dn) {
    set_error_handler(array('LDAPInterface', 'void_error_handler'));
    $ret = ldap_delete($this->connection, $dn);
    restore_error_handler();

    return $ret;
  }
  
  // This function is used by other modules to delete attributes once they are
  // moved to profiles cause ldap_mod_del does not delete facsimileTelephoneNumber if
  // attribute value to delete is passed to the function. 
  // OpenLDAP as per RFC 2252 doesn't have equality matching for facsimileTelephoneNumber
  // http://bugs.php.net/bug.php?id=7168
  function deleteAttribute($dn, $attribute) {
    ldap_mod_del($this->connection, $dn, array($attribute => array()));
  }
}

?>
