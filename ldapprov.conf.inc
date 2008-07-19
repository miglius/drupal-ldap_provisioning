<?php
// $Id$

/**
 * @file
 * Module additional configuration.
 */

/**
 * Edit this list so that all additional attributes that you might wish to show
 * on the registration form are included. You may then enable, disable or set
 * mandatory attributes in the LDAP Provisioning Settings page.
 *
 * The type of element may be 'textifield' or 'textarea'.
 *
 * Return an empty array if no additional elements are needed.
 */
function ldapprov_custom_elements() {
  return  array(
    'remarks' => array(
      'type' => 'textarea',
      'title' => t('Remarks'),
      'weigth' => 3,
      'rows' => 6,
      'description' => t('Additional registration remarks.'),
    ),
    'sponsor' => array(
      'type' => 'textfield',
      'title' => t('Sponsor POC'),
      'weigth' => 4,
      'size' => 64,
      'maxlength' => 64,
      'description' => t('Sponsor Point of Contact.'),
    ),
  );
}

