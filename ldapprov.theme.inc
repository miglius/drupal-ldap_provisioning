<?php
// $Id$

/**
 * @file
 * Themes for ldapprov.
 */

//////////////////////////////////////////////////////////////////////////////
// Theme callbacks

/**
 * List account requests form theme.
 */
function theme_ldapprov_list_form($form) {

  // Overview table
  $header = $form['status']['#value'] != 3 ? array(theme('table_select_header_cell')) : array();
  $header = array_merge($header, $form['header']['#value']);
  unset($form['header']);
  $output = drupal_render($form['options']);
  if (isset($form['mail']) && is_array($form['mail'])) {
    foreach (element_children($form['mail']) as $key) {
      $row = array();
      if ($form['status']['#value'] != 3) {
        $row[] = drupal_render($form['requests'][$key]);
      }
      $row[] = drupal_render($form['registered'][$key]);
      if (isset($form['name'])) {
        $row[] = drupal_render($form['name'][$key]);
      }
      $row[] = drupal_render($form['first_name'][$key]);
      $row[] = drupal_render($form['last_name'][$key]);
      $row[] = drupal_render($form['mail'][$key]);
      if (module_exists('ldapprov_invite') && LDAPPROV_INVITE_ENABLED)
        $row[] = drupal_render($form['inviter'][$key]);
      if (isset($form['approved']))
        $row[] = drupal_render($form['approved'][$key]);
      if (isset($form['name_c']))
        $row[] = drupal_render($form['name_c'][$key]);
      if (isset($form['name_u']))
        $row[] = drupal_render($form['name_u'][$key]);
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array('data' => t('No entries available.'), 'colspan' => count($header)));
  }

  $output .= theme('table', $header, $rows);
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);

  return $output;
}

