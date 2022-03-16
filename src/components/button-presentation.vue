<template>
  <div class="presentation">
    <div>
      <h4>color and text</h4>
      <butt
        color="magenta"
        text="test"
      />
      <butt 
        color="blue"
        text="test"
      />
      <butt
        color="green"
        text="test"
      />
    </div>
    <div>
      <h4>color and icon</h4>
      <butt
        icon="./assets/icon-button.svg"
        color="magenta"
      />
      <butt
        icon="./assets/icon-button.svg"
        color="blue"
      />
      <butt
        icon="./assets/icon-button.svg"
        color="green"
      />
    </div>
    <div>
      <h4>color, icon, text, icon position</h4>
      <butt
        icon="./assets/icon-button.svg"
        color="magenta"
        text="test"
      />
      <butt
        icon="./assets/icon-change.svg"
        color="blue"
        text="test"
      />
      <butt
        icon="./assets/icon-cancel.svg"
        color="green"
        text="reverse"
        icon_position="right"
      />
    </div>
    <div>
      <h4>transparent</h4>
      <butt 
        icon="./assets/icon-receive.svg" 
        transparent
      />
      <butt 
        icon="./assets/icon-receive.svg"
        text="test" 
        transparent
        text_color="blue"
      />
      <butt 
        icon="./assets/icon-receive.svg"
        text="test" 
        transparent
        text_color="blue"
        icon_position="right"
      />
      <butt 
        text="test"
        transparent
        text_color="blue"
      />
      <div class="block">
        <butt 
        icon="./assets/icon-receive.svg" 
        transparent
      />
      <butt 
        icon="./assets/icon-copy.svg" 
        transparent
      />
      <butt 
        icon="./assets/icon-key.svg" 
        transparent
      />
      <butt 
        icon="./assets/icon-change.svg" 
        transparent
      />
      </div>
    </div>
    <div>
      <h4>semi transparent</h4>
      <butt 
        icon="./assets/icon-change.svg" 
        text="semi transparent"
        semi_transparent
        text_color="white"
      />
    </div>
    <div>
      <h4>disabled</h4>
      <butt 
        icon="./assets/icon-change.svg" 
        text="DISABLED and this is a long phrase just to show that everything is alright"
        color="green"
        disabled
      />
    </div>
  </div>
</template>

<script>
import butt from './button.vue'

export default {
  components: { butt },
}
</script>

<style scoped lang="stylus">
.presentation {
  width: 100%
  height: 100%
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
