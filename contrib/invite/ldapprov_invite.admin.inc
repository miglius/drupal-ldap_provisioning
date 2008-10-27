<?php
// $Id$

/**
 * @file
 * Module admin page callbacks.
 */

//////////////////////////////////////////////////////////////////////////////
// ldapprov_invite settings

/**
 * Implements the settings page.
 *
 * @return
 *   The form structure.
 */
function ldapprov_invite_admin_settings() {
  // Invites
  $form['settings'] = array('#type' => 'fieldset', '#title' => t('Settings'), '#collapsible' => TRUE, '#collapsed' => FALSE);
  $form['settings']['ldapprov_invite_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable LDAP invites'),
    '#default_value' => LDAPPROV_INVITE_ENABLED,
  );
  $form['settings']['ldapprov_invite_from'] = array(
    '#type' => 'radios',
    '#title' => t('"From" e-mail address'),
    '#default_value' => LDAPPROV_INVITE_FROM,
    '#options' => array(t('Inviter'), t('Site')),
    '#description' => t('Choose which e-mail address will be in the From: header for the invitation mails sent. %site will use the default e-mail address of the site, whereas %inviter will use the e-mail address of the person who is sending the invitation.', array('%site' => t('Site'), '%inviter' => t('Inviter'))),
  );
  $form['invite'] = array('#type' => 'fieldset', '#title' => t('Invite email'), '#collapsible' => TRUE, '#collapsed' => FALSE);
  $form['invite']['ldapprov_invite_mail_invite_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject of invite e-mail'),
    '#maxlength' => 180,
    '#default_value' => _ldapprov_invite_mail_text('invite_subject'),
    '#description' => t('Customize the subject of your invite e-mail message.') .' '. t('Available variables are:') .' !site, !name, !register_uri, !mailto, !date.',
  );
  $form['invite']['ldapprov_invite_mail_invite_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body of invite e-mail'),
    '#rows' => 15,
    '#default_value' => _ldapprov_invite_mail_text('invite_body'),
    '#description' => t('Customize the body of your invite e-mail message.') .' '. t('Available variables are:') .' !site, !name, !register_uri, !mailto, !date.',
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
function ldapprov_invite_admin_settings_validate($form, &$form_state) {
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
function ldapprov_invite_admin_settings_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#value'];
  switch ($op) {
    case t('Save configuration'):
      $values = $form_state['values'];

      variable_set('ldapprov_invite_enabled', $values['ldapprov_invite_enabled']);
      variable_set('ldapprov_invite_from', $values['ldapprov_invite_from']);
      variable_set('ldapprov_invite_mail_invite_subject', $values['ldapprov_invite_mail_invite_subject']);
      variable_set('ldapprov_invite_mail_invite_body', $values['ldapprov_invite_mail_invite_body']);

      drupal_set_message(t('The configuration options have been saved.'));
      break;
    case t('Reset to defaults'):

      variable_del('ldapprov_invite_enabled');
      variable_del('ldapprov_invite_from');
      variable_del('ldapprov_invite_mail_invite_subject');
      variable_del('ldapprov_invite_mail_invite_body');

      drupal_set_message(t('The configuration options have been reset to their default values.'));
      break;
  }

  // Rebuild the menu router.
  menu_rebuild();
}

