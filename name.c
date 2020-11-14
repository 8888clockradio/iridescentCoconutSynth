/*
        Copyright 2020 George C. Rosar II licensed under:
                 Apache License
           Version 2.0, January 2004
        http://www.apache.org/licenses/
        
        Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


#include "usb_names.h"

#define MANUFACTURER_NAME  {'i','r','i','d','e','s', 'c', 'e', 'n', 't'}
#define MANUFACTURER_NAME_LEN 10

#define MIDI_NAME   {'i','r','i','d','e','s','c','e','n','t',' ','C','o','c','o','n','u','t'}
#define MIDI_NAME_LEN  18

// Do not change this part.  This exact format is required by USB.

struct usb_string_descriptor_struct usb_string_product_name = {
        2 + MIDI_NAME_LEN * 2,
        3,
        MIDI_NAME
};

struct usb_string_descriptor_struct usb_string_manufacturer_name = {
  2 + MANUFACTURER_NAME_LEN * 2,
  3,
  MANUFACTURER_NAME
};

struct usb_string_descriptor_struct usb_string_serial_number = {
  12,
  3,
  {
    0,0,7,0,0,0,7,0,0,7  }
};
