<template>
  <div  :class="['container', id === 'name' ? 'name_input' : '']">
    <label v-if="label" class="label_elem">{{ label }}</label>
    <i class="icon" v-if="imgName">
            <img :src="`../assets/${imgName}.svg`" alt="icon"/>
       </i>
    <input
        v-bind="$attrs"
        :value="modelValue"
        :placeholder="placeholder"
        :style="style"
        @input="$emit('update:modelValue', $event.target.value)"
        :class="['input_elem', isValid ? '' : 'error_elem']"
    >
  </div>
</template>

<style scoped lang="stylus">
  .container {
     margin-top: 20px
     width: 100%

    .label_elem {
      display: block
      margin-bottom:10px
    }

    .name_input {
      margin-bottom:74px
    }

    i {
      position: absolute;
    }

    .icon {
      padding: 15px;
    }
    
    .error_elem {
     color:red!important;
    }

    .input_elem {
    background-color: rgba(255, 255, 255, .05)
    border: none
    font-size: 24px
    resize: none
    color: #0bccf7
    height: 100%
    padding: 0
    border-radius: 10px
    padding: 8px

    &:focus {
        outline-width: 0
    }

    &::placeholder {
        font-size: 14px
        color: rgba(255, 255, 255, 0.3)
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
    imgName: {
      type: String,
      required: false,
    },
    paddingLeft: {
      type: String,
      required: false
    },
    width: {
      type: String,
      required: false
    },
    height: {
      type: String,
      required: false
    }
  },

      computed: {
      style() {
        return {
            "padding-left": this.paddingLeft + 'px',
             width: this.width + 'px',
             height: this.height + 'px'
          }
      }
    },

}
</script>
