<template>
  <div class="container">
    <label v-if="label" class="label" :class="{'error': !valid}">
      {{ label }}
    </label>
    <div class="input-container">
      <img v-if="img" :src="`../assets/${img}.svg`" :class="{'error': !valid}" alt="icon"/>
      <input :value="modelValue"
             :placeholder="placeholder"
             :style="style"
             :class="{'input': true, 'error': !valid}"
             @input="$emit('update:modelValue', $event.target.value)"
      />
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .container {
    box-sizing: border-box

    .label {
      display: block
      margin-bottom:10px
      color: rgba(255, 255, 255, 0.6)
      font-family: 'SFProDisplay', sans-serif
      font-size: 14px

      &.error {
        color: rgba(255, 98, 92, 0.7)
      }
    }

    .input-container {
      display: flex

      & > img {
        position: absolute
        padding: 15px 15px

        &.error {
          filter: grayscale(100%) brightness(40%) sepia(100%) hue-rotate(-50deg) saturate(600%) contrast(0.8)
        }
      }
    }

    .input {
      font-family: 'SFProDisplay', sans-serif
      background-color: rgba(255, 255, 255, 0.05)
      border: none
      outline-width: 0
      font-size: 14px
      color: white
      border-radius: 10px
      padding: 12px 8px
      width: 100%

      &:not(.error):focus {
        background-color: rgba(255, 255, 255, 0.1)
      }

      &.error:focus {
        background-color: rgba(255, 98, 92, 0.12)
      }

      &.error {
        color: rgba(255, 98, 92, 1)
        background-color: rgba(255, 98, 92, 0.07)
      }

      &:not(.error)::placeholder {
        font-size: 14px
        color: rgba(255, 255, 255, 0.3)
      }

      &.error::placeholder {
        font-size: 14px
        color: rgba(255, 98, 92, 0.4)
      }
    }
  }
</style>

<script>
export default {
  props: {
    label: {
      type: String,
      default: '',
      required: false
    },
    placeholder: {
      type: String,
      default: '',
      required: false
    },
    // eslint-disable-next-line vue/prop-name-casing
    modelValue: {
      type: String,
      default: '',
      required: true
    },
    valid: {
      type: Boolean,
      default: true,
      required: false
    },
    img: {
      type: String,
      default:'',
      required: false,
    }
  },

  emits: [
    'update:modelValue'
  ],

  computed: {
    style() {
      return {
        'padding-left': this.img.length ? '38px' : '15px'
      }
    }
  },
}
</script>
