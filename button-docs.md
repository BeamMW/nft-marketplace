# Button component has the following properties:
  * [color](#color)
  * [transparent](#transparent)
  * [semi_transparent](#semi`_`transparent)
  * [icon](#icon)
  * [icon_right](#icon_right)
  * [text](#text)
  * [text_color](#text-color)
  * [gap_default](#gap-default)
  * [gap_value](#gap-value)
  * [margin_left_default](#margin-left-default)
  * [margin_left_value](#margin-left-value)
  * [no_hover](#no-hover)
#
# color
### Sets the value of `background-color`:
    <BUTTON
      color="magenta"
    />
### Available colors are in `style` section `button.vue`.
#
# transparent
### Sets the `background-color` value transparent:
    <BUTTON
      transparent
    />
#
# semi_transparent
### Sets the value of `background-color` to white with 10% opacity:
    <BUTTON
      semi_transparent
    />
#
# icon
### Allows you to add an icon:
    <BUTTON
      icon="./assets/icon-beam.svg"
    />
#
# icon_position
### If button has text and an icon, puts the icon on the `right` of the text: 
    <BUTTON
      icon_right
    />
### Default value is `left`. If there is only an icon, it will be `centered`.
#
# text
### Allows to add text to the button:
    <BUTTON
      text="Press me"
    />
#
# text_color
### Sets the color for the text:
    <BUTTON
      text_color="white"
    />
### Available colors are in `data` section of `button.vue`.
#
# gap_default
### Sets a default value between two inner elements:
    <BUTTON
      gap_default
    />
### Default value is in `style` section of `button.vue`.
#
# gap_value
### Allows to explicitly set a value for gap between two inner elements:
    <BUTTON
      gap_value="50"
    />
### Takes a `number` type value.
#
# margin_left_default
### Sets the default value of `margin-left` for the `button`:
    <BUTTON
      margin_left_default
    />
#
# margin_left_value
### Allow to set a value of `margin-left` for the `button`:
    <BUTTON
      margin_left_value="50"
    />
### Takes a `number` type value.
#
# no_hover
### Removes `hover` effects from the `button`:
    <BUTTON
      no_hover
    />
