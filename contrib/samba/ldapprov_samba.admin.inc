<?php
// $Id$

/**
 * @file
 * Module admin page callbacks.
 */

//////////////////////////////////////////////////////////////////////////////
// ldapprov_samba settings

/**
 * Implements the settings page.
 *
 * @return
 *   The form structure.
 */
function ldapprov_samba_admin_settings() {
  // Samba extensions
  $form['samba'] = array('#type' => 'fieldset', '#title' => t('Samba extensions'), '#collapsible' => TRUE, '#collapsed' => FALSE);
  $form['samba']['ldapprov_samba_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Samba extensions'),
    '#default_value' => LDAPPROV_SAMBA_ENABLED,
  );
  $form['samba']['ldapprov_samba_entry'] = array(
    '#type' => 'textarea',
    '#title' => t('Samba LDAP attributes'),
    '#default_value' => LDAPPROV_SAMBA_ENTRY,
    '#rows' => 20,
    '#description' => t('Customize the LDAP Samba attributes in LDIF style. Available variables are: !uid_number, !gid_number, !name, !samba_sid, !primary_group_id, !pwd_next_timestamp, !pwd_timestamp, !lm_pass, !nt_pass. Empty lines and lines starting with "#" will be ignored.'),
  );
  $form['samba']['ldapprov_samba_domain_sid'] = array(
    '#type' => 'textfield',
    '#title' => t('Samba Domain SID'),
    '#default_value' => LDAPPROV_SAMBA_DOMAIN_SID,
    '#size' => 50,
    '#maxlength' => 255,
    '#description' => t("Type 'net getlocalsid' on your Samba server to get SID."),
  );
  $form['samba']['ldapprov_samba_default_group'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Samba Group'),
    '#default_value' => LDAPPROV_SAMBA_DEFAULT_GROUP,
    '#size' => 50,
    '#maxlength' => 255,
    '#description' => t("Name (cn) of the default Samba group for all new users."),
  );
  $form['samba']['ldapprov_samba_group_base_dn'] = array(
    '#type' => 'textfield',
    '#title' => t('Samba Groups\' Base DN'),
    '#default_value' => LDAPPROV_SAMBA_GROUP_BASE_DN,
    '#size' => 50,
    '#maxlength' => 255,
    '#description' => t("Base DN of the Samba groups. Leave it empty if it is the same as LDAP server's Base DN."),
  );
  $form['samba']['ldapprov_samba_pass_expire'] = array(
    '#type' => 'textfield',
    '#title' => t('Samba Password Expiration'),
    '#default_value' => LDAPPROV_SAMBA_PASS_EXPIRE,
    '#size' => 50,
    '#maxlength' => 255,
    '#description' => t("Samba password expiration period in days."),
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
function ldapprov_samba_admin_settings_validate($form, &$form_state) {
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
function ldapprov_samba_admin_settings_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#value'];
  switch ($op) {
    case t('Save configuration'):
      $values = $form_state['values'];

      variable_set('ldapprov_samba_enabled', $values['ldapprov_samba_enabled']);
      variable_set('ldapprov_samba_entry', $values['ldapprov_samba_entry']);
      variable_set('ldapprov_samba_domain_sid', $values['ldapprov_samba_domain_sid']);
      variable_set('ldapprov_samba_default_group', $values['ldapprov_samba_default_group']);
      variable_set('ldapprov_samba_group_base_dn', $values['ldapprov_samba_group_base_dn']);
      variable_set('ldapprov_samba_pass_expire', $values['ldapprov_samba_pass_expire']);

      drupal_set_message(t('The configuration options have been saved.'));
      break;
    case t('Reset to defaults'):

      variable_del('ldapprov_samba_enabled');
      variable_del('ldapprov_samba_entry');
      variable_del('ldapprov_samba_domain_sid');
      variable_del('ldapprov_samba_default_group');
      variable_del('ldapprov_samba_group_base_dn');
      variable_del('ldapprov_samba_pass_expire');

      drupal_set_message(t('The configuration options have been reset to their default values.'));
      break;
  }
}

