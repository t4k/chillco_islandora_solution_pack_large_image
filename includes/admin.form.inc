<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function chillco_islandora_solution_pack_large_image_admin(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/solution_packs');

  drupal_set_message('Use will upload a tiff/jpeg which will get converted to the appropriate derivatives. Will not use jpeg2000 until further compatibility.', 'warning');
  $form['chillco_islandora_solution_pack_large_image_lossless'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create Lossless Derivatives'),
    '#default_value' => variable_get('chillco_islandora_solution_pack_large_image_lossless', 0),
    '#description' => t('Lossless derivatives are of higher quality but adversely affect browser performance.'),
  );

  $form['chillco_islandora_solution_pack_large_image_use_kakadu'] = array(
    '#type' => 'checkbox',
    '#title' => t("Use Kakadu for image compression"),
    '#default_value' => variable_get('chillco_islandora_solution_pack_large_image_use_kakadu', 0),
    '#description' => t("!kakadu offers faster derivative creation than the standard ImageMagick package.", array(
                      '!kakadu' => l(t('Kakadu'), 'http://www.kakadusoftware.com/'),
                    )),
  );
  
  $form['islandora_large_image_uncompress_tiff'] = array(
    '#type' => 'checkbox',
    '#title' => t('Uncompress TIFF files prior to creating JP2 datastreams'),
    '#description' => t('The version of Kakadu shipped with djatoka does not ' .
                    'support compressed TIFFs; therefore, it is likely desirable to ' .
                    'uncompress the TIFF so Kakadu does not encounter an error. This will ' .
                    'not change the original TIFF stored in the OBJ datastream. Only ' .
                    'disable this if you are completely sure!'),
    '#default_value' => variable_get('islandora_large_image_uncompress_tiff', TRUE),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_use_kakadu"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['chillco_islandora_solution_pack_large_image_use_kakadu_url'] = array(
    '#type' => 'textfield',
    '#title' => t("Path to Kakadu"),
    '#default_value' => variable_get('chillco_islandora_solution_pack_large_image_use_kakadu_url', '/usr/bin/kdu_compress'),
    '#description' => t('Path to the kdu_compress executable.<br/>!msg', array('!msg' => islandora_executable_available_message(variable_get('chillco_islandora_solution_pack_large_image_use_kakadu_url', '/usr/bin/kdu_compress')))),
    '#prefix' => '<div id="kakadu-wrapper">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'callback' => 'chillco_islandora_solution_pack_large_image_update_kakadu_url_div',
      'wrapper' => 'kakadu-wrapper',
      'effect' => 'fade',
      'event' => 'blur',
      'progress' => array('type' => 'throbber'),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_use_kakadu"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form += islandora_viewers_form('chillco_islandora_solution_pack_large_image_viewers', 'image/jp2');

  return system_settings_form($form);
}

function chillco_islandora_solution_pack_large_image_update_kakadu_url_div($form, $form_state) {
  return $form['islandora_kakadu_url'];
}
