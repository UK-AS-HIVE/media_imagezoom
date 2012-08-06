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

  for ($zoom_level = 0; $zoom_level < 6; ++$zoom_level) {
    drush_print('Computing zoom level ' . $zoom_level . "...\n");

    //for ($fid = 1; $fid<$file_count; ++$fid) {
    foreach ($fids as $delta => $fid) {
      $start_time = microtime(TRUE);

      print("...image $fid (level $zoom_level) ($delta of " . count($fids) . " - " . intval($delta*100.0/count($fids)) . "%)");
      //_media_imagezoom_generate_zoomlevel_tiles($fid, $zoom_level);
      drush_media_imagezoom_generate_zoomlevel_tiles_method2($fid, $zoom_level);

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

/**
 *  Create tiles using ImageMagick commandline utilities instead of Drupal ImageAPI
 */
function drush_media_imagezoom_generate_zoomlevel_tiles_method2($fid, $zoom_level)
{
  // if already exists, don't recreate
	$pow2 = pow(2.0,$zoom_level);
	$max_tile_name = 'public://tiles/tms/1.0.0/' . $fid . '/' . $zoom_level . '/' . ($zoom_level==0?'0':(intval($pow2-1))) . '/' . ($zoom_level==0?'0':intval($pow2-1)) . '.jpg';
  if (file_exists($max_tile_name)) {
    return;
  }

	$media = file_load($fid);

	if ($media->type != 'image') {
		return;
	}

  //$start_resize_time = microtime(TRUE);

	$image = image_load($media->uri);
	$full_tile_width = max($image->info['width'],$image->info['height']) / pow(2.0,$zoom_level);
	
	// squarify
	image_crop($image, 0,0, max($image->info['width'],$image->info['height']), max($image->info['width'],$image->info['height']), TRUE );
	image_resize($image, 256*$pow2, 256*$pow2);

	// save temp
	//@file_delete(drupal_realpath('public://tiles/tmpResizedForCutting.jpg'), TRUE);
	image_save($image, 'public://tiles/tmpResizedForCutting.jpg');

  //$elapsed_resize_time = microtime(TRUE) - $start_resize_time;
  //print("(resize took " . $elapsed_resize_time . " seconds)\n");


  // now batch crop with image magic
  //print('attempting: convert ' . drupal_realpath('public://tiles/tmpResizedForCutting.jpg') . ' -crop 256x256 ' . drupal_realpath('public://tiles/tmpTile.jpg') . "\n");
  exec('convert ' . drupal_realpath('public://tiles/tmpResizedForCutting.jpg') . ' -crop 256x256 ' . drupal_realpath('public://tiles/tmpTile.jpg'));
	
  $i = 0;
	//for ($y = 0; $y < $pow2; ++$y)
	for ($y = $pow2-1; $y >= 0; --$y)
	for ($x = 0; $x < $pow2; ++$x)
	{
		
		$tile_name = 'public://tiles/tms/1.0.0/' . $fid . '/' . $zoom_level . '/' . $x . '/' . $y . '.jpg';

		//$tile_cut = image_load('public://tiles/tmpResizedForCutting.jpg');

		//image_crop($tile_cut, $x*256, ($pow2-$y-1)*256, 256, 256);
		@drupal_mkdir(dirname($tile_name), NULL, TRUE, NULL);
    if ($zoom_level == 0) {
      //print('mv ' . drupal_realpath('public://tiles/tmpTile.jpg') . ' ' . drupal_realpath($tile_name) . "\n");
      rename(drupal_realpath('public://tiles/tmpTile.jpg'), drupal_realpath($tile_name));
    } else {
      //print('mv ' . drupal_realpath('public://tiles/tmpTile-'.$i.'.jpg') . ' ' . drupal_realpath($tile_name) . "\n");
      rename(drupal_realpath('public://tiles/tmpTile-'.$i.'.jpg'), drupal_realpath($tile_name));
    }

    $i++;

		//$image->source = $tile_name;
		//image_save($tile_cut, $tile_name);
	}

}

