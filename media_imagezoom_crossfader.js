jQuery(document).ready(function() {
  var map = Drupal.settings.openlayers.maps[jQuery('.openlayers-map').attr('id')];
  jQuery('.openlayers-map').prepend('<div id="lichfieldLayerFader" class="olControlNoSelect" style="position: relative; left: 50px; z-index: 1001; height: 0;"  unselectable="on"></div>');
  var baseSelect = '<select class="baseLayerSelect">';
  var opacitySlider = '<div id="opacitySlider" style="display: inline-block; width: 160px; font-size: 10px; margin: 5px;"></div>';
  var overlaySelect = '<select class="overlayLayerSelect">';
  //for (var i=0; i<map.layers.length; ++i)
  var i = 0;
  for (layername in map.layers) {
    if (map.layers[layername].isBaseLayer) {
      baseSelect += '<option value="' + i + '">' + map.layers[layername].title + '</option>';
    } else {
      overlaySelect += '<option value="' + i + '">' + map.layers[layername].title + '</option>';
    }
    i++;
  }
  baseSelect += '</select>';
  overlaySelect += '</select>';
  jQuery('#lichfieldLayerFader').prepend(overlaySelect);
  jQuery('#lichfieldLayerFader').prepend(opacitySlider);
  jQuery('#lichfieldLayerFader').prepend(baseSelect);

  jQuery('#opacitySlider').slider({value:50, slide: function(event,ui) {setLayerOpacity(ui.value / 100.0);} });

  jQuery('select.baseLayerSelect').change(function() {
    var ol = jQuery('#' + jQuery('.openlayers-map').attr('id')).data('openlayers').openlayers;
    ol.setBaseLayer(ol.layers[jQuery(this).val()]);
  });
  jQuery('select.overlayLayerSelect').change(function() {
    var openLayersId = jQuery('.openlayers-map').attr('id');
    var mapLayers = jQuery('#' + openLayersId).data('openlayers').openlayers.layers;
    var selectedLayerIndex = jQuery(this).val();
    for (i in mapLayers) {
      if (!mapLayers[i].isBaseLayer) {
        mapLayers[i].setVisibility(i == selectedLayerIndex);
      }
    }
  });
});

function setLayerOpacity(opacity) {
  //var mapLayers = Drupal.settings.openlayers.maps[jQuery('.openlayers-map').attr('id')];
  var mapLayers = jQuery('#' + jQuery('.openlayers-map').attr('id')).data('openlayers').openlayers.layers;
  for (i in mapLayers) {
    if (!mapLayers[i].isBaseLayer) {
      mapLayers[i].setOpacity(opacity);
    }
  }
}
