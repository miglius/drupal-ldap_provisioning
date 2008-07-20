<?php
// $Id$

/**
 * @file
 * Module admin page callbacks.
 */

//////////////////////////////////////////////////////////////////////////////
// ldapprov settings

/**
 * Implements the settings page.
 *
 * @return
 *   The form structure.
 */
function ldapprov_admin_settings() {
  // General settings
  $form['general'] = array('#type' => 'fieldset', '#title' => t('General settings'), '#collapsible' => TRUE, '#collapsed' => FALSE);
  $form['general']['ldapprov_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable LDAP user provisioning'),
    '#default_value' => LDAPPROV_ENABLED,
  );
  $form['general']['ldapprov_allow_username'] = array(
    '#type' => 'radios',
    '#title' => t('Custom username'),
    '#options' => array('1' => t('Yes'), '0' => t('No')),
    '#default_value' => LDAPPROV_ALLOW_USERNAME,
    '#description' => t('Allow to choose custom username in the registration form.'),
  );
  $form['general']['ldapprov_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username template'),
    '#size' => 50,
    '#maxlength' => 255,
    '#default_value' => LDAPPROV_USERNAME,
    '#description' => t('Customize the username.') .' '. t('Available variables are:') .' %f ('. t('first name') .'), %f1 ('. t('first letter of first name, etc') .'), %l ('. t('last name') .'), %l2 ('. t('first two letters of last name, etc') .').',
  );
  if (module_exists('captcha')) {
    $result = db_query("SELECT * FROM {captcha_points} WHERE form_id = 'ldapprov_register'");
    $captcha = ($row = db_fetch_object($result)) ? 1 : 0;
    $form['general']['ldapprov_captcha'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use captcha in the registration form.'),
      '#default_value' => $captcha,
    );
  }
  $form['general']['ldapprov_disable_create'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable drupal user creation'),
    '#description' => t('Whether the drupal user creation form for admin users should be disabled.'),
    '#default_value' => LDAPPROV_DISABLE_CREATE,
  );
  $form['general']['ldapprov_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable debugging'),
    '#description' => t('The LDIF entry will be print on the screen for configuration debugging.'),
    '#default_value' => LDAPPROV_DEBUG,
  );

  // Registration form.
  $form['registration'] = array('#type' => 'fieldset', '#title' => t('Registration form'), '#collapsible' => TRUE, '#collapsed' => TRUE);
  if (module_exists('og')) {
    $form['registration']['ldapprov_og'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show groups in the registration form'),
      '#default_value' => LDAPPROV_OG,
      '#description' => t('Groups are shown on the registration form according to their setup.'),
    );
  }
  if (module_exists('profile')) {
    $form['registration']['ldapprov_profile'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use profile fields in the registration form'),
      '#default_value' => LDAPPROV_PROFILE,
      '#description' => t('Profile fields are shown on the registration form according to their setup in the profile.'),
    );
    $form['registration']['ldapprov_profile_first'] = array(
      '#type' => 'textfield',
      '#title' => t('Profile first name'),
      '#size' => 50,
      '#maxlength' => 255,
      '#default_value' => LDAPPROV_PROFILE_FIRST,
      '#description' => t('Profile field associated with the user\'s first name. When the field is set and custom usernames are not allowed, this field will be used to construct a username.'),
    );
    $form['registration']['ldapprov_profile_last'] = array(
      '#type' => 'textfield',
      '#title' => t('Profile last name'),
      '#size' => 50,
      '#maxlength' => 255,
      '#default_value' => LDAPPROV_PROFILE_LAST,
      '#description' => t('Profile field associated with the user\'s last name. When the field is set and custom usernames are not allowed, this field will be used to construct a username.'),
    );
  }
  if (module_exists('ldapdata')) {
    $server = _ldapprov_get_server();
    $rwattrs = unserialize($server->ldapdata_rwattrs);
    if (!empty($rwattrs)) {
      $options = array('not shown', 'optional', 'required');
      $ldapdata_defaults = variable_get('ldapprov_ldapdata', array());
      $ldapdata_attributes = ldapdata_attributes();
      foreach ($rwattrs as $attribute) {
        if ($attribute != LDAPPROV_LDAP_FIRST_NAME && $attribute != LDAPPROV_LDAP_LAST_NAME) {
          $attr_info = $ldapdata_attributes[$attribute];
          $form['registration']['ldapprov_ldapdata_'. $attribute] = array(
            '#type' => 'select',
            '#title' => t($attr_info[2]),
            '#options' => $options,
            '#default_value' => $ldapdata_defaults[$attribute],
            '#description' => t('LDAP attribute %attr.', array('%attr' => $attribute)),
          );
        }
      }
    }
  }
  include_once(drupal_get_path('module', 'ldapprov') .'/ldapprov.conf.inc');
  $ldapprov_custom = variable_get('ldapprov_custom', array());
  $options = array('not shown', 'optional', 'required');
  foreach (ldapprov_custom_elements() as $key => $val) {
    $form['registration']['ldapprov_custom_'. $key] = array(
      '#type' => 'select',
      '#title' => t($val['title']),
      '#options' => $options,
      '#description' => t($val['title'] .' field on the registration form.'),
      '#default_value' => $ldapprov_custom[$key],
    );
  }

  // LDAP authentication.
  $form['ldap'] = array('#type' => 'fieldset', '#title' => ('LDAP authentication'), '#collapsible' => TRUE, '#collapsed' => TRUE);
  $ldap_servers = array();
  $result = db_query("SELECT sid, name, status FROM {ldapauth} WHERE status = '1' ORDER BY sid");
  while ($row = db_fetch_object($result)) {
    $ldap_servers[$row->sid] = $row->name;
  }
  $form['ldap']['ldapprov_server'] = array(
    '#type' => 'select',
    '#title' => t('LDAP server'),
    '#options' => $ldap_servers,
    '#default_value' => LDAPPROV_SERVER,
    '#description' => t('Select LDAP server to create LDAP accounts in.'),
  );
  $form['ldap']['ldapprov_dn'] = array(
    '#type' => 'textfield',
    '#title' => t('Bind DN'),
    '#size' => 50,
    '#maxlength' => 255,
    '#default_value' => LDAPPROV_DN,
    '#description' => t('Bind DN should have the rights to create new LDAP entries.'),
  );

  if (LDAPPROV_PASS_CLEAR || !LDAPPROV_PASS) {
    variable_del('ldapprov_pass');
    $form['ldap']['ldapprov_pass'] = array(
      '#type' => 'password',
      '#title' => t('Password'),
      '#size' => 50,
      '#maxlength' => 255,
      '#default_value' => '',
    );
    variable_set('ldapprov_pass_clear', FALSE);
  }
  else {
    $form['ldap']['ldapprov_pass_clear'] = array(
      '#type' => 'checkbox',
      '#title' => t('Clear current password'),
      '#default_value' => FALSE,
    );
  }

  // LDAP fields.
  $form['ldap_attributes'] = array('#type' => 'fieldset', '#title' => t('LDAP attributes'), '#collapsible' => TRUE, '#collapsed' => TRUE);
  $form['ldap_attributes']['ldapprov_user_entry'] = array(
    '#type' => 'textarea',
    '#title' => t('Basic LDAP attributes'),
    '#default_value' => variable_get('ldapprov_user_entry', "objectClass: top\nobjectClass: person\nobjectClass: inetOrgPerson\nuid: %uid\nmail: %mail\ngivenName: %first_name\nsn: %last_name\ncn: %first_name %last_name\nuserPassword: %pass"),
    '#rows' => 15,
    '#default_value' => LDAPPROV_USER_ENTRY,
    '#description' => t('Customize the rest LDAP attributes in LDIF style.') .' '. t('Available variables are:') .' %uid, %base_dn, %mail, %first_name, %last_name, %pass. '. t('Empty lines and lines starting with "#" will be ignored.'),
  );

  // E-mail notification.
  $form['mail'] = array('#type' => 'fieldset', '#title' => t('E-mail notification'), '#collapsible' => TRUE, '#collapsed' => TRUE);
  $form['mail']['ldapprov_mail_code_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject of validate e-mail'),
    '#maxlength' => 180,
    '#default_value' => _ldapprov_mail_text('code_subject'),
    '#description' => t('Customize the subject of your request validate e-mail message, which is sent to new members upon registering.') .' '. t('Available variables are:') .' !site, !validate_url, !validate_uri, !mailto, !first_name, !last_name, !date, !code.',
  );
  $form['mail']['ldapprov_mail_code_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body of validate e-mail'),
    '#rows' => 15,
    '#default_value' => _ldapprov_mail_text('code_body'),
    '#description' => t('Customize the body of your request validate e-mail message, which is sent to new members upon registering.') .' '. t('Available variables are:') .' !site, !validate_url, !validate_uri, !mailto, !first_name, !last_name, !date, !code.',
  );
  $form['mail']['ldapprov_mail_notify_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject of notify e-mail'),
    '#maxlength' => 180,
    '#default_value' => _ldapprov_mail_text('notify_subject'),
    '#description' => t('Customize the subject of your account manager notification e-mail message.') .' '. t('Available variables are:') .' !site, !mailto, !first_name, !last_name, !date, !create_url.',
  );
  $form['mail']['ldapprov_mail_notify_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body of notify e-mail'),
    '#rows' => 15,
    '#default_value' => _ldapprov_mail_text('notify_body'),
    '#description' => t('Customize the body of your account manager notification e-mail message.') .' '. t('Available variables are:') .' !site, !mailto, !first_name, !last_name, !date, !create_url.',
  );
  $form['mail']['ldapprov_mail_reject_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject of reject e-mail'),
    '#maxlength' => 180,
    '#default_value' => _ldapprov_mail_text('reject_subject'),
    '#description' => t('Customize the subject of your request reject e-mail message.') .' '. t('Available variables are:') .' !site, !mailto, !first_name, !last_name, !date, !message.',
  );
  $form['mail']['ldapprov_mail_reject_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body of reject e-mail'),
    '#rows' => 15,
    '#default_value' => _ldapprov_mail_text('reject_body'),
    '#description' => t('Customize the body of your request reject e-mail message.') .' '. t('Available variables are:') .' !site, !mailto, !first_name, !last_name, !date, !message.',
  );
  $form['mail']['ldapprov_mail_create_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject of creation e-mail'),
    '#maxlength' => 180,
    '#default_value' => _ldapprov_mail_text('create_subject'),
    '#description' => t('Customize the subject of new account creation notification e-mail message.') .' '. t('Available variables are:') .' !site, !login_uri, !login_url, !mailto, !first_name, !last_name, !date, !username, !password, !message.',
  );
  $form['mail']['ldapprov_mail_create_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body of creation e-mail'),
    '#rows' => 15,
    '#default_value' => _ldapprov_mail_text('create_body'),
    '#description' => t('Customize the body of new account creation notification e-mail message.') .' '. t('Available variables are:') .' !site, !login_uri, !login_url, !mailto, !first_name, !last_name, !date, !username, !password, !message.',
  );
  $form['mail']['ldapprov_mail_delete_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject of deletion e-mail'),
    '#maxlength' => 180,
    '#default_value' => _ldapprov_mail_text('delete_subject'),
    '#description' => t('Customize the subject of the account deletion notification e-mail message.') .' '. t('Available variables are:') .' !site, !mailto, !first_name, !last_name, !date, !username.',
  );
  $form['mail']['ldapprov_mail_delete_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body of deletion e-mail'),
    '#rows' => 15,
    '#default_value' => _ldapprov_mail_text('delete_body'),
    '#description' => t('Customize the body of the account deletion notification e-mail message.') .' '. t('Available variables are:') .' !site, !mailto, !first_name, !last_name, !date, !username.',
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
function ldapprov_admin_settings_validate($form, &$form_state) {
  $op = $form_state['clicked_button']['#value'];
  switch ($op) {
    case "Save configuration":
      $values = $form_state['values'];
      // Check the profile first and last names
      if (module_exists('profile') && !LDAPPROV_ALLOW_USERNAME) {
        if (!empty($values['ldapprov_profile_first'])) {
          $result = db_query("SELECT * FROM {profile_fields} WHERE name = '%s'", $values['ldapprov_profile_first']);
          $row = db_fetch_object($result);
          if (empty($row)) {
            form_set_error('ldapprov_profile_first', t('Profile field %s is not found.', array('%s' => $values['ldapprov_profile_first'])));
          }
          elseif ($row->required == 0 || $row->register == 0) {
            form_set_error('ldapprov_profile_first', t('Profile field %s should be configured as "', array('%s' => $values['ldapprov_profile_first'])) . t('Visible in user registration form') . t('" and "') . t('The user must enter a value') .'".');
          }
        }
        if (!empty($values['ldapprov_profile_last'])) {
          $result = db_query("SELECT * FROM {profile_fields} WHERE name = '%s'", $values['ldapprov_profile_last']);
          $row = db_fetch_object($result);
          if (empty($row)) {
            form_set_error('ldapprov_profile_last', t('Profile field %s is not found.', array('%s' => $values['ldapprov_profile_last'])));
          }
          elseif ($row->required == 0 || $row->register == 0) {
            form_set_error('ldapprov_profile_last', t('Profile field %s should be configured as "', array('%s' => $values['ldapprov_profile_last'])) . t('Visible in user registration form') . t('" and "') . t('The user must enter a value') .'".');
          }
        }
        if (!empty($values['ldapprov_profile_first']) && empty($values['ldapprov_profile_last'])) {
          form_set_error('ldapprov_profile_last', t('If profile first name is set, the last name should be set as well.'));
        }
        if (!empty($values['ldapprov_profile_last']) && empty($values['ldapprov_profile_first'])) {
          form_set_error('ldapprov_profile_first', t('If profile last name is set, the first name should be set as well.'));
        }
      }
      break;
  }
}

/**
 * Submit hook for the settings form.
 */
function ldapprov_admin_settings_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#value'];
  switch ($op) {
    case "Save configuration":
      $values = $form_state['values'];

      // General settings.
      variable_set('ldapprov_enabled', $values['ldapprov_enabled']);
      variable_set('ldapprov_allow_username', $values['ldapprov_allow_username']);
      variable_set('ldapprov_username', $values['ldapprov_username']);

      if (module_exists('captcha')) {
        $result = db_query("SELECT * FROM {captcha_points} WHERE form_id = 'ldapprov_register'");
        $row = db_fetch_object($result);
        if ($values['ldapprov_captcha'] == 1 && empty($row)) {
          db_query("INSERT INTO {captcha_points} (form_id) VALUES ('ldapprov_register')");
        }
        elseif ($values['ldapprov_captcha'] == 0 && !empty($row)) {
          db_query("DELETE FROM {captcha_points} WHERE form_id = 'ldapprov_register'");
        }
      }

      variable_set('ldapprov_disable_create', $values['ldapprov_disable_create']);
      variable_set('ldapprov_debug', $values['ldapprov_debug']);

      // OG.
      if (module_exists('og')) {
        variable_set('ldapprov_og', $values['ldapprov_og']);
      }

      // Profiles.
      if (module_exists('profile')) {
        variable_set('ldapprov_profile', $values['ldapprov_profile']);
        variable_set('ldapprov_profile_first', $values['ldapprov_profile_first']);
        variable_set('ldapprov_profile_last', $values['ldapprov_profile_last']);
      }

      // Registration form.
      $ldapprov_custom = array();
      include_once(drupal_get_path('module', 'ldapprov') .'/ldapprov.conf.inc');
      foreach (ldapprov_custom_elements() as $key => $val) {
        $ldapprov_custom[$key] = $values['ldapprov_custom_'. $key];
      }
      variable_set('ldapprov_custom', $ldapprov_custom);

      // LDAP authentication.
      variable_set('ldapprov_server', $values['ldapprov_server']);
      variable_set('ldapprov_dn', $values['ldapprov_dn']);
      if (!empty($values['ldapprov_pass'])) {
        variable_set('ldapprov_pass', $values['ldapprov_pass']);
      }
      if ($values['ldapprov_pass_clear'] == 1) {
        variable_set('ldapprov_pass', '');
      }

      // LDAP fields.
      if (module_exists('ldapdata')) {
        $ldapprov_ldapdata = array();
        foreach ($values as $key => $val) {
          if (preg_match("/^ldapprov_ldapdata_/", $key)) {
            $ldapprov_ldapdata[preg_replace("/^ldapprov_ldapdata_/", '', $key)] = $val;
          }
        }
        variable_set('ldapprov_ldapdata', $ldapprov_ldapdata);
      }
      variable_set('ldapprov_user_entry', $values['ldapprov_user_entry']);

      // E-mail notification.
      variable_set('ldapprov_mail_code_subject', $values['ldapprov_mail_code_subject']);
      variable_set('ldapprov_mail_code_body', $values['ldapprov_mail_codee_body']);
      variable_set('ldapprov_mail_notify_subject', $values['ldapprov_mail_notify_subject']);
      variable_set('ldapprov_mail_notify_body', $values['ldapprov_mail_notify_body']);
      variable_set('ldapprov_mail_reject_subject', $values['ldapprov_mail_reject_subject']);
      variable_set('ldapprov_mail_reject_body', $values['ldapprov_mail_reject_body']);
      variable_set('ldapprov_mail_create_subject', $values['ldapprov_mail_create_subject']);
      variable_set('ldapprov_mail_create_body', $values['ldapprov_mail_create_body']);
      variable_set('ldapprov_mail_delete_subject', $values['ldapprov_mail_delete_subject']);
      variable_set('ldapprov_mail_delete_body', $values['ldapprov_mail_delete_body']);

      drupal_set_message(t('The configuration options have been saved.'));
      break;
    case "Reset to defaults":
      $values = $form_state['values'];

      // General settings.
      variable_del('ldapprov_enabled');
      variable_del('ldapprov_allow_username');
      variable_del('ldapprov_username');

      if (module_exists('captcha')) {
        db_query("DELETE FROM {captcha_points} WHERE form_id = 'ldapprov_register'");
      }

      variable_del('ldapprov_disable_create');
      variable_del('ldapprov_debug');

      variable_del('ldapprov_og');

      variable_del('ldapprov_profile');
      variable_del('ldapprov_profile_first');
      variable_del('ldapprov_profile_last');

      // Registration form.
      variable_del('ldapprov_custom'. $key);

      // LDAP authentication.
      variable_del('ldapprov_server');
      variable_del('ldapprov_dn');
      variable_del('ldapprov_pass');
      variable_del('ldapprov_pass_clear');

      // LDAP fields.
      variable_del('ldapprov_ldapdata');
      variable_del('ldapprov_user_entry');

      // E-mail notification.
      variable_del('ldapprov_mail_code_subject');
      variable_del('ldapprov_mail_code_body');
      variable_del('ldapprov_mail_notify_subject');
      variable_del('ldapprov_mail_notify_body');
      variable_del('ldapprov_mail_reject_subject');
      variable_del('ldapprov_mail_reject_body');
      variable_del('ldapprov_mail_create_subject');
      variable_del('ldapprov_mail_create_body');
      variable_del('ldapprov_mail_delete_subject');
      variable_del('ldapprov_mail_delete_body');

      drupal_set_message(t('The configuration options have been reset to their default values.'));
      break;
  }

  // Rebuild the menu router.
  menu_rebuild();
}
