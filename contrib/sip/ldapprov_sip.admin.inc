<?php
// $Id$

/**
 * @file
 * Module admin page callbacks.
 */

//////////////////////////////////////////////////////////////////////////////
// ldapprov_sip settings

/**
 * Implements the settings page.
 *
 * @return
 *   The form structure.
 */
function ldapprov_sip_admin_settings() {
  // SIP extensions
  $form['sip'] = array('#type' => 'fieldset', '#title' => t('SIP extensions'), '#collapsible' => TRUE, '#collapsed' => FALSE);
  $form['sip']['ldapprov_sip_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable SIP extensions'),
    '#default_value' => LDAPPROV_SIP_ENABLED,
  );
  $form['sip']['ldapprov_sip_name'] = array(
    '#type' => 'textfield',
    '#title' => t('SIP name'),
    '#default_value' => LDAPPROV_SIP_NAME,
    '#size' => 50,
    '#maxlength' => 255,
    '#description' => t('Customize the SIP name !sip_name variable used in SIP attributes. Available variables are: !name (drupal user name), !ldap_ou (LDAP organizational unit ou).'),
  );
  $form['sip']['ldapprov_sip_entry'] = array(
    '#type' => 'textarea',
    '#title' => t('SIP LDAP attributes'),
    '#default_value' => LDAPPROV_SIP_ENTRY,
    '#rows' => 20,
    '#description' => t('Customize the LDAP SIP attributes in LDIF style. Available variables are: !sip_name, !sip_pass, !sip_voicemail_pass. Empty lines and lines starting with "#" will be ignored.'),
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['reset'] = array(
    '#type'  => 'submit',
    '#value' => t('Reset to defaults'),
  );
  return $form;
}

/**
 * Validate hook for the settings form.
 */
function ldapprov_sip_admin_settings_validate($form, &$form_state) {
  $op = $form_state['clicked_button']['#value'];
  switch ($op) {
    case t('Save configuration'):
      $values = $form_state['values'];
      break;
  }
}

/**
 * Submit hook for the settings form.
 */
function ldapprov_sip_admin_settings_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#value'];
  switch ($op) {
    case t('Save configuration'):
      $values = $form_state['values'];

      variable_set('ldapprov_sip_enabled', $values['ldapprov_sip_enabled']);
      variable_set('ldapprov_sip_name', $values['ldapprov_sip_name']);
      variable_set('ldapprov_sip_entry', $values['ldapprov_sip_entry']);

      drupal_set_message(t('The configuration options have been saved.'));
      break;
    case ('Reset to defaults'):

      variable_del('ldapprov_sip_enabled');
      variable_del('ldapprov_sip_name');
      variable_del('ldapprov_sip_entry');

      drupal_set_message(t('The configuration options have been reset to their default values.'));
      break;
  }
}

