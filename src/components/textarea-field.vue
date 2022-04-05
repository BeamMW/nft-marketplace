<template>
  <div class="container">
    <label v-if="label" class="label">{{ label }}</label>
    <textarea v-bind="$attrs" class="input"
              :value="modelValue"
              :placeholder="placeholder"
              :maxlength="max_length"
              :readonly="readonly"
              @input="$emit('update:modelValue', $event.target.value)"
    >
  </textarea>
    <div :class="{
      'info': true,
      'readonly': readonly,
    }"
    >
      <span class="text">{{ max_length }} characters max</span> 
      <span v-if="show_counter">{{ modelValue.length }} / {{ max_length }} </span>
    </div>
  </div>
</template>

<style scoped lang="stylus">
.container {
  width: 100%
  box-sizing: border-box

  .label {
    display: block
    margin-bottom:10px
    color: rgba(255, 255, 255, 0.6)
    font-family: 'SFProDisplay', sans-serif
    font-size: 14px
  }

  .input {
    box-sizing: border-box
    font-family: 'SFProDisplay', sans-serif
    background-color: rgba(255, 255, 255, 0.08)
    border: none
    outline-width: 0
    font-size: 14px
    color: white
    height: 100%
    padding: 0
    border-radius: 10px
    padding: 12px 13px 
    width: 100%
    resize: none
    min-height: 79px

    &:read-only {
      background-color: rgba(255, 255, 255, 0.03)
      color: rgba(255, 255, 255, 0.3)
    }

    &::placeholder {
      font-size: 14px
      color: rgba(255, 255, 255, 0.3)
    }

    &:not(:read-only):focus {
      background-color: rgba(255, 255, 255, 0.12)
      outline-width: 0
    }
  }

  .info {
    display: flex
    justify-content: space-between
    font-style: italic
    font-weight: 400
    font-size: 12px

    &.readonly {
      color: rgba(255, 255, 255, 0.35)
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
    readonly: {
      type: Boolean,
      default: false,
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
    valid:{
      type: Boolean,
      required: true
    },
    max_length:{
      type: Number,
      default: 150,
      required: true
    },
    show_counter:{
      type: Boolean,
      default: false,
      required: true
    }
  },
  
  emits: [
    'update:modelValue'
  ],
}
</script>
