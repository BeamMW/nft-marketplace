<template>
  <div class="switch-container">
    <label class="switch">
      <input type="checkbox"
             :checked="modelValue"
             @change="$emit('update:modelValue', $event.target.checked)"
      >
      <span class="slider round"></span>
    </label>
    <label class="switch_text">{{ label }}</label>
  </div>
</template>

<style scoped lang="stylus">
.switch-container {
  display: flex
  align-items: flex-end

  .switch {
    position: relative
    display: inline-block
    width: 29px
    height: 19px
              
    .slider {
      position: absolute
      cursor: pointer
      top: 0
      left: 0
      right: 0
      bottom: 0
      background-color: rgba(255, 255, 255, 0.05)
      transition: 0.4s
      border: 1px solid #b4bec8

      &::before {
        position: absolute
        content: ""
        height: 17px
        width: 16px
        left: 0px
        bottom: 0px
        background-color: #b4bec8
        transition: 0.4s
      }
    }
    .round {
      border-radius: 34px

      &:before {
        border-radius: 50%
      }
    }

    input {
      opacity: 0
      width: 0
      height: 0
      
      &:checked + .slider {
        background-color: rgba(26, 246, 214, 0.35)
      }

      &:focus + .slider {
        box-shadow: 0 0 1px #2196f3
      }

      &:checked + .slider:before {
        transform: translateX(11px)
      }
    }
  }

  .switch_text {
    margin-left:10px
    font-size: 14px
    line-height: 17px
    color: rgba(255, 255, 255, 0.7)
  }
}
</style>

<script>
export default {
  props: {
    // eslint-disable-next-line vue/prop-name-casing
    modelValue: {
      type: Boolean,
      required: true,
    },
    label: {
      type: String,
      default: '',
    },
  },
  emits: [
    'update:modelValue'
  ]
}
</script>
