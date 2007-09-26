; $Id$

LDAP User provisioning module provides a user registration process in Drupal and creates user accounts in both LDAP server and Drupal. It depends on the ldapauth module in the ldap_integration package.


FEATURES
--------

- E-mail address validation. When a new user fills in a registration form, an email with the secret code and following instructions is mailed out to his e-mail address to validate existence of the submitted e-mail address. Only when user validates his e-mail address an account is created or registration data is set as pending (put in the approval queue) based on User registration settings.
- Approval queue. When User settings are set that administrator approval is required, then upon registration e-mail is sent out to all users with 'create accountsâ permissions notifying about a new account request. The account manager then can review the registration data submitted and choose to create or reject the account. He can also leave an internal message for other account managers.
- Username building from a configurable template. It is possible to set a template from which a username will be generated from first and last names in case we don't want to allow custom usernames.
- Invites. Users with 'invite friends' permission can send invitations to the site to other people. When user is registering from the invite the e-mail validation and approval queue are skipped and account is created when user fills the registration form. The user, which sent out the invite, is set as the approver of the account.
- Batch user upload. The module provides downloadable csv and xml templates created on the fly with all required registration fields marked. The data of new users can be entered into the file and upload back to the site. The uploaded file is processed, the data is extracted and all people from the list are put into pending accounts list.
- Multiple account creation in one step. Account manager can select multiple account requests in the pending accounts list and create all new accounts in one step. Each account's data is passed to the registration form validation function and account is created if it passes it. If it fails account manager is asked to create account manually since registration data should be changed to pass validation.
- Custom additional registration fields. Administrator can set custom additional registration fields - text fields or text areas.
- Logging of user creation actions. Along with each account request the time of the filling in the registration form, the approver (who created or rejected the request), and approval date is saved in the database for further reference.


INTEGRATION
-----------

- ldapdata module. When ldapdata module has writable LDAP fields configured, provisioning module allows printing those fields n the registration form.
- profile module. Provisioning module respects profile field settings and prints fields, which are configured to appear on registration form.
- buddylist module. When inviting other people to the website, one can choose if he wants that user to appear under his buddy list upon registration.
- captcha module. Provisioning module can be configured to add captcha point on the registration form.


INSTALLATION
____________

First make sure that ldapauth module is proper configured and working. Then just add this module to the modules folder, enable it and configure to meet your needs.


AUTHOR
------

Miglius Alaburda
miglius at gmail dot com
