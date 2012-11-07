<?php
/**
 * @file
 * Adds 'precache-media-tiles' command to drush
 */

/**
 * Implements hook_drush_command().
 */
function media_imagezoom_precache_drush_command() {
  $items = array();
  $items['precache-media-tiles'] = array(
    'description' => "Runs through all Media images and makes sure TMS tiles for them are generated and cached on disk",
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  return $items;
}

/**
 * Callback for drush 'precache-media-tiles' command.
 */
function drush_media_imagezoom_precache_precache_media_tiles() {
  $q = db_select('file_managed', 'f');
  $q->fields('f', array('fid'));
  $q->condition('f.status', FILE_STATUS_PERMANENT);
  $q->condition('f.uri', 'public://%', 'LIKE');
  $fids = $q->execute()->fetchCol();

  for ($zoom_level = 0; $zoom_level < 6; ++$zoom_level) {
    drush_print('Computing zoom level ' . $zoom_level . "...\n");

    foreach ($fids as $delta => $fid) {
      $start_time = microtime(TRUE);

      print("...image $fid (level $zoom_level) ($delta of " . count($fids) . " - " . intval($delta*100.0/count($fids)) . "%)");
      // Slower: _media_imagezoom_generate_zoomlevel_tiles($fid, $zoom_level);
      drush_media_imagezoom_generate_zoomlevel_tiles_method2($fid, $zoom_level);

      $elapsed_time = microtime(TRUE) - $start_time;
      print('(took ' . $elapsed_time . " seconds)");
      print("              \r");
      
    }
  }
}

/**
 * Create tiles using ImageMagick commandline utilities instead of Drupal ImageAPI
 * This is significantly faster in most cases
 */
function drush_media_imagezoom_generate_zoomlevel_tiles_method2($fid, $zoom_level) {
  $pow2 = pow(2.0,$zoom_level);
  $max_tile_name = 'public://tiles/tms/1.0.0/' . $fid . '/' . $zoom_level . '/' . ($zoom_level==0?'0':(intval($pow2-1))) . '/' . ($zoom_level==0?'0':intval($pow2-1)) . '.jpg';

  // If already exists, don't recreate
  if (file_exists($max_tile_name)) {
    return;
  }

  $media = file_load($fid);

  if ($media->type != 'image') {
    return;
  }

  $image = image_load($media->uri);
  $full_tile_width = max($image->info['width'],$image->info['height']) / pow(2.0,$zoom_level);
  
  // Squarify
  image_crop($image, 0,0, max($image->info['width'],$image->info['height']), max($image->info['width'],$image->info['height']), TRUE );
  image_resize($image, 256*$pow2, 256*$pow2);

  // Save temp
  image_save($image, 'public://tiles/tmpResizedForCutting.jpg');

  // Now batch crop with ImageMagick on the commandline
  exec('convert ' . drupal_realpath('public://tiles/tmpResizedForCutting.jpg') . ' -crop 256x256 ' . drupal_realpath('public://tiles/tmpTile.jpg'));
  
  $i = 0;

  for ($y = $pow2 - 1; $y >= 0; --$y)
  for ($x = 0; $x < $pow2; ++$x) {
    
    $tile_name = 'public://tiles/tms/1.0.0/' . $fid . '/' . $zoom_level . '/' . $x . '/' . $y . '.jpg';

    @drupal_mkdir(dirname($tile_name), NULL, TRUE, NULL);
    if ($zoom_level == 0) {
      rename(drupal_realpath('public://tiles/tmpTile.jpg'), drupal_realpath($tile_name));
    } else {
      rename(drupal_realpath('public://tiles/tmpTile-'.$i.'.jpg'), drupal_realpath($tile_name));
    }

    $i++;
  }

}
