<?php

/**
 * Creates the derivatives for this content model type
 *
 * @param AbstractObject $object
 *   The object to add derivatives to.
 * @param bool $force
 *   Whether or not we should force the derivatives to be created, in the case
 *   that they already exist on the given object.
 *
 * @return boolean
 *   TRUE if all derivatives were created successfully, FALSE otherwise.
 */
function chillco_islandora_solution_pack_large_image_create_all_derivatives(AbstractObject $object, $force = TRUE) {
  if (!isset($object['OBJ']) && !isset($object['IMAGE'])) {
    drupal_set_message(t('Did not create image derivatives for %s. No tiff/image file was uploaded.', array('%s' => $object->id)), 'error');
    return FALSE;
  }
  $base_name = str_replace(':', '-', $object->id);
  $uploaded_file = chillco_islandora_solution_pack_large_image_get_uploaded_image_file($object, $base_name);
  if ($uploaded_file) {
    $large = chillco_islandora_solution_pack_large_image_create_jpg_derivative($object, $uploaded_file, $base_name, $force, 'LARGE', 'chillco_islandora_solution_pack_large_image_large');
    $medium = chillco_islandora_solution_pack_large_image_create_jpg_derivative($object, $uploaded_file, $base_name, $force, 'MEDIUM', 'chillco_islandora_solution_pack_large_image_medium');
    $tn = chillco_islandora_solution_pack_large_image_create_jpg_derivative($object, $uploaded_file, $base_name, $force, 'TN', 'chillco_islandora_solution_pack_large_image_tn');
    return $large && $medium && $tn && file_unmanaged_delete($uploaded_file);
  }
  return FALSE;
}

function chillco_islandora_solution_pack_large_image_get_uploaded_image_file(AbstractObject $object, $base_name) {
  $convert = FALSE;
  if (isset($object['IMAGE'])) {
    $current_object = $object['IMAGE'];
  }
  else if (isset($object['OBJ'])) {
    $convert = TRUE;
    $current_object = $object['OBJ'];
  }
  else {
    return FALSE;
  }
  $mime_detector = new MimeDetect();
  $ext = $mime_detector->getExtension($current_object->mimeType);
  $filename = file_create_filename("{$base_name}_ORIGINAL.{$ext}", 'temporary://');
  $current_object->getContent($filename);
  // Create conversion and ingest.
  if ($convert) {
    $new_filename = file_create_filename("{$base_name}.IMAGE.jpg", 'temporary://');
    _imagemagick_convert($filename, $new_filename, array());
    file_unmanaged_delete($filename);
    $filename = $new_filename;
    $file_path = drupal_realpath($filename);
    chillco_islandora_solution_pack_large_image_add_datastream($object, 'IMAGE', $file_path, file_get_mimetype($file_path), basename($file_path));
  }
  return $filename;
}

function chillco_islandora_solution_pack_large_image_create_jpg_derivative(AbstractObject $object, $uploaded_file, $base_name, $force, $dsid, $style_name) {
  if ($force || !isset($object[$dsid])) {
    $filename = file_create_filename("{$base_name}.{$dsid}.jpg", 'temporary://');
    $image_style = image_style_load($style_name);
    if(image_style_create_derivative($image_style, $uploaded_file, $filename)) {
      $file_path = drupal_realpath($filename);
      chillco_islandora_solution_pack_large_image_add_datastream($object, $dsid, $file_path, file_get_mimetype($file_path), basename($file_path));
    }
    return file_unmanaged_delete($filename);
  }
  return TRUE;
}

function chillco_islandora_solution_pack_large_image_add_datastream(AbstractObject $object, $dsid, $file, $mimetype, $label) {
  $ds = $object->constructDatastream($dsid, 'M');
  $ds->label = $label;
  $ds->mimeType = $mimetype;
  $ds->setContentFromFile($file);
  try {
    $object->ingestDatastream($ds);
  } catch (exception $e) {
    drupal_set_message(t('@message', array('@message' => check_plain($e->getMessage()))), 'error');
    return FALSE;
  }
  return TRUE;
}
