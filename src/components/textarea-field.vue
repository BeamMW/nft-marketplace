<template>
  <div :class="['area-container', id === 'name' ? 'name_input' : '']">
    <label v-if="label" class="label_elem">{{ label }}</label>
    <i v-if="imgName" class="icon">
      <img :src="`../assets/${imgName}.svg`" alt="icon"/>
    </i>
    <textArea v-bind="$attrs"
              :value="modelValue"
              :placeholder="placeholder"
              :style="style"
              :class="['input_elem', isValid ? '' : 'error_elem']"
              @input="$emit('update:modelValue', $event.target.value)"
    >
  </textarea>
    <div class="info">
      <span class="text">{{ maxLength }} characters max</span> 
      <span>{{ modelValue.length }} / {{ maxLength }} </span>
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .area-container {
     width: 100%
     box-sizing: border-box

    .label_elem {
      display: block
      margin-bottom:10px
    }

    .error_elem {
     color:red!important;
    }

    .input_elem {
      background-color: rgba(255, 255, 255, .05)
      border: none
      font-size: 24px
      color: #0bccf7
      height: 100%
      padding: 0
      border-radius: 10px
      padding: 8px
      width: 100%
      resize: none
      min-height: 79px

      &:focus {
        outline-width: 0
      }

      &::placeholder {
        font-size: 14px
        color: rgba(255, 255, 255, 0.3)
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
    modelValue: {
      type: String,
      default: '',
      required: true
    },
    id:{
      type: String,
      default: '',
      required: true
    },
    isValid:{
      type: Boolean,
      required: true
    },
    maxLength:{
      type: Number,
      default: 150,
      required: true
    }
  },
  emits: {'update:modelValue': null},
}
</script>
