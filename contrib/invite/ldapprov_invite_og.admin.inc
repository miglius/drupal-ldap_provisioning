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

      drupal_set_message(t('The configuration options have been saved.'));
      break;
    case t('Reset to defaults'):

      variable_del('ldapprov_invite_og_tab');

      drupal_set_message(t('The configuration options have been reset to their default values.'));
      break;
  }

  // Rebuild the menu router.
  menu_rebuild();
}

