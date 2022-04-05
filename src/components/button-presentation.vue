<template>
  <div class="presentation">
    <div>
      <h4>color and text</h4>
      <btn color="magenta" text="test"/>
      <btn color="blue" text="test"/>
      <btn color="green" text="test"/>
    </div>

    <div>
      <h4>color and icon</h4>
      <btn color="magenta">
        <img src="~/assets/icon-button.svg"/>
      </btn>
      <btn color="blue">
        <img src="~/assets/icon-button.svg"/>
      </btn>
      <btn color="green">
        <img src="~/assets/icon-button.svg"/>
      </btn>  
    </div>

    <div>
      <h4>color, icon, text, icon position</h4>
      <btn color="magenta" text="test">
        <img src="~/assets/icon-button.svg"/>
      </btn>
      <btn color="blue" text="test">
        <img src="~/assets/icon-button.svg"/>
      </btn>
      <btn color="green" text="reverse" reverse>
        <img src="~/assets/icon-button.svg"/>
      </btn>
    </div>

    <div>
      <h4>transparent</h4>
      <btn color="transparent">
        <img src="~/assets/icon-receive.svg"/>
      </btn>
      <btn text="test" text_color="blue" color="transparent">
        <img src="~/assets/icon-receive.svg"/>
      </btn>
      <btn text="test" color="transparent" text_color="blue" reverse>
        <img src="~/assets/icon-receive.svg"/>
      </btn>
      <btn text="test" color="transparent" text_color="blue"/>

      <div class="block">
        <btn color="transparent">
          <img src="~assets/icon-copy.svg"/>
        </btn>
        <btn color="transparent">
          <img src="~assets/icon-key.svg"/>
        </btn>
      </div>
    </div>
    
    <div>
      <h4>default</h4>
      <btn text="semi transparent">
        <img src="~/assets/icon-change.svg"/>
      </btn>
    </div>

    <div>
      <h4>height, width, padding</h4>
      <btn text="heigth" height="50px">
        <img src="~/assets/icon-change.svg"/>
      </btn>
      <btn text="width" width="200px">
        <img src="~/assets/icon-change.svg"/>
      </btn>
      <btn text="padding & height" padding="0px 60px" height="20px">
        <img src="~/assets/icon-change.svg"/>
      </btn>
    </div>
    
    <div>
      <h4>disabled</h4>
      <btn text="DISABLED and this is a long phrase just to show that everything is alright"
           color="green"
           disabled
      >
        <img src="~/assets/icon-change.svg"/>
      </btn>
    </div>
  </div>
</template>

<style scoped lang="stylus">
.presentation {
  width: 100%
  min-height: 100%
  margin-left: 10px
  padding-top: 20px
  display: flex
  flex-wrap: wrap
  justify-content: flex-start

  div {
    margin-right: 30px
  }

  button {
    margin-bottom: 10px
  }

  .block {
    display: flex

    button {
      margin-right: 5px
    }
  }
}
</style>

<script>
import btn from './button.vue'

export default {
  components: {btn},
}
</script>
