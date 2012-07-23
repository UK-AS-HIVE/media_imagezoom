<?php

function media_imagezoom_precache_drush_command() {
  $items = array();
  $items['precache-media-tiles'] = array(
    'description' => "Runs through all Media images and makes sure TMS tiles for them are generated and cached on disk",
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  return $items;
}

function drush_media_imagezoom_precache_precache_media_tiles() {
  $q = db_select('file_managed', 'f');
  $q->fields('f', array('fid'));
  $q->condition('f.status', FILE_STATUS_PERMANENT);
  $q->condition('f.uri', 'public://%', 'LIKE');
  $fids = $q->execute()->fetchCol();
  

  for ($zoom_level = 0; $zoom_level < 5; ++$zoom_level) {
    drush_print('Computing zoom level ' . $zoom_level . '...');

    //for ($fid = 1; $fid<$file_count; ++$fid) {
    foreach ($fids as $delta => $fid) {
      $start_time = microtime(TRUE);

      print("...image $fid (level $zoom_level) ($delta of " . count($fids) . " - " . intval($delta*100.0/count($fids)) . "%)");
      _media_imagezoom_generate_zoomlevel_tiles($fid, $zoom_level);

      $elapsed_time = microtime(TRUE) - $start_time;
      print('(took ' . $elapsed_time . " seconds)");
      print("              \r");

      /*$dim = pow(2,$zoom_level);
      for ($x = 0; $x < $dim; ++$x)
        for ($y = 0; $y < $dim; ++$y)
          _media_imagezoom_tile($fid, $zoom_level, $x, $y);*/
    }
  }
}

