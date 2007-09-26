<?php
/* $Id$ */

// Edit this list so that all additional attributes that you might wish to show
// on the registration form are included. You may then enable, disable or set
// mandatory attributes in the LDAP Provisioning Settings page.
//
// The left hand element indicates machine readable name without spaces.
// The type of element may be 'textifield' or 'textarea'.
global $_ldapprov_custom;
$_ldapprov_custom = array(
  'remarks' => array(
    'type' => 'textarea',
    'title' => t('Remarks'),
    'description' => t('Additional registration remarks.'), 
    'weigth' => 3,
    'rows' => 6,
  ),
  'sponsor' => array(
    'type' => 'textfield',
    'title' => t('Sponsor POC'),
    'description' => t('Sponsor Point of Contact.'),
    'weigth' => 4,
    'size' => 64,
    'maxlength' => 64,
  ),
);

