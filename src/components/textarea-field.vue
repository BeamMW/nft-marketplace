<template>
  <div class="container">
    <label v-if="label" class="label">{{ label }}</label>
    <textarea v-bind="$attrs"
              :value="modelValue"
              :placeholder="placeholder"
              class="input"
              :maxlength="max_length"
              @input="$emit('update:modelValue', $event.target.value)"
    >
  </textarea>
    <div class="info">
      <span class="text">{{ max_length }} characters max</span> 
      <span>{{ modelValue.length }} / {{ max_length }} </span>
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
      background-color: rgba(255, 255, 255, 0.05)
      border: none
      outline-width: 0
      font-size: 14px
      color: white
      height: 100%
      padding: 0
      border-radius: 10px
      padding: 12px 15px 
      width: 100%
      resize: none
      min-height: 79px

      &::placeholder {
        font-size: 14px
        color: rgba(255, 255, 255, 0.3)
      }

      &:focus {
        background-color: rgba(255, 255, 255, 0.1)
        outline-width: 0
      }
    }

    .info {
      display: flex
      justify-content: space-between
      font-style: italic
      font-weight: 400
      font-size: 12px
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
    valid:{
      type: Boolean,
      required: true
    },
    max_length:{
      type: Number,
      default: 150,
      required: true
    }
  },
  
  emits: [
    'update:modelValue'
  ],
}
</script>
