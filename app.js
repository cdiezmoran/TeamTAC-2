var UI = require('ui');
var Vector2 = require('vector2');
//var ajax = require('ajax');
var light = require('ui/light');
var Settings = require('settings');

//var eppTLink = "https://xcapapi.kodiaknetworks.com/JSCDE_shape/eptt.min.js";
//var jsType = "javascript";



/*ajax({ url: epptLink, type:jsType
     },
);*/
Settings.config(
  { url: 'http://www.example.com' },
  function(e) {
    console.log('opening configurable');

    // Reset color to red before opening the webview
    Settings.option('color', 'red');
  },
  function(e) {
    console.log('closed configurable');
  }
  );

var main = new UI.Card({
  title: 'SOS App',
  icon: 'images/menu_icon.png',
  subtitle: 'Emergency',
  body: 'Press any button.',
  subtitleColor: 'red', // Named colors
  bodyColor: '#9a0036' // Hex colors
});

main.show();

main.on('click', 'select', function(e) {
  var wind = new UI.Window({backgroundColor: 'black'});
  var textfield = new UI.Text({size: new Vector2(140, 60),font: 'gothic-24-bold',text: 'Sending Alert ',
  textAlign: 'center',
    color: 'green'
  });
  var windSize = wind.size();
  // Center the textfield in the window
  var textfieldPos = textfield.position()
      .addSelf(windSize)
      .subSelf(textfield.size())
      .multiplyScalar(0.5);
  textfield.position(textfieldPos);
  wind.add(textfield);
  wind.show();
  light.trigger();
  wind.on('click', 'up', function (){
        light.trigger();
        console.log("Set request!");
      });
});
