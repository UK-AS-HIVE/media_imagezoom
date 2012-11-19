<?php

/**
 * @file
 * Provide administration screens for media_imagezoom module.
 */

/**
 * Implements form callback
 */
function media_imagezoom_admin_form() {
  $form['tile_path'] = array(
    '#type' => 'textfield',
    '#title' => 'Tile storage URI',
    '#description' => 'Base path for tile storage.  All generated tiles will be stored in subdirectories here.',
    '#default_value' => variable_get('media_imagezoom_tilepath','public://tiles'),
    '#required' => TRUE,
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#validate' => array('media_imagezoom_admin_form_validate'),
    '#submit' => array('media_imagezoom_admin_form_submit'),
    '#value' => t('Save'),
  );

  return $form;
}

function media_imagezoom_admin_form_validate($form, &$form_state) {
  if (!file_stream_wrapper_get_instance_by_uri(trim($form_state['values']['tile_path']))) {
    form_set_error('tile_path', 'Invalid or unrecognized base URI for tile storage.  Something like public://tiles is recommended.');
  }
}

function media_imagezoom_admin_form_submit($form, &$form_state) {
  $tile_path = trim($form_state['values']['tile_path']);

  if (file_stream_wrapper_get_instance_by_uri($tile_path)) {
    variable_set('media_imagezoom_tilepath', $tile_path);
  }
}
