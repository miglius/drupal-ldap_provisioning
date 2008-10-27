<?php
// $Id$

/**
 * @file
 * Module admin page callbacks.
 */

//////////////////////////////////////////////////////////////////////////////
// ldapprov_invite_og settings

/**
 * Implements the settings page.
 *
 * @return
 *   The form structure.
 */
function ldapprov_invite_og_admin_settings() {
  // visibility
  $form['visibility'] = array('#type' => 'fieldset', '#title' => t('Visibility'), '#collapsible' => TRUE, '#collapsed' => FALSE);
  $form['visibility']['ldapprov_invite_og_tab'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable tab on the group\'s homepage'),
    '#default_value' => LDAPPROV_INVITE_OG_TAB,
    '#description' => t('If enabled, the tab will be shown on the group\'s homepage. Otherwise this functionality will be available from the menu.'),
  );

  $form['invite'] = array('#type' => 'fieldset', '#title' => t('Invite email'), '#collapsible' => TRUE, '#collapsed' => FALSE);
  $form['invite']['ldapprov_invite_og_mail_invite_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject of invite e-mail'),
    '#maxlength' => 180,
    '#default_value' => _ldapprov_invite_og_mail_text('invite_subject'),
    '#description' => t('Customize the subject of your invite e-mail message.') .' '. t('Available variables are:') .' !site, !name, !register_uri, !mailto, !date, !group.',
  );
  $form['invite']['ldapprov_invite_og_mail_invite_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body of invite e-mail'),
    '#rows' => 15,
    '#default_value' => _ldapprov_invite_og_mail_text('invite_body'),
    '#description' => t('Customize the body of your invite e-mail message.') .' '. t('Available variables are:') .' !site, !name, !register_uri, !mailto, !date, !group.',
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
function ldapprov_invite_og_admin_settings_validate($form, &$form_state) {
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
function ldapprov_invite_og_admin_settings_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#value'];
  switch ($op) {
    case t('Save configuration'):
      $values = $form_state['values'];

      variable_set('ldapprov_invite_og_tab', $values['ldapprov_invite_og_tab']);
      variable_set('ldapprov_invite_og_mail_invite_subject', $values['ldapprov_invite_og_mail_invite_subject']);
      variable_set('ldapprov_invite_og_mail_invite_body', $values['ldapprov_invite_og_mail_invite_body']);

      drupal_set_message(t('The configuration options have been saved.'));
      break;
    case t('Reset to defaults'):

      variable_del('ldapprov_invite_og_tab');
      variable_del('ldapprov_invite_og_mail_invite_subject');
      variable_del('ldapprov_invite_og_mail_invite_body');

      drupal_set_message(t('The configuration options have been reset to their default values.'));
      break;
  }

  // Rebuild the menu router.
  menu_rebuild();
}

