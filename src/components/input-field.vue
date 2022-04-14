<template>
  <div class="input-container">
    <label v-if="label" class="label" :class="{'err': !valid}">
      {{ label }}
    </label>
    <div class="input">
      <img v-if="img" :src="`../assets/${img}.svg`" :class="{'error': !valid}" alt="icon"/>
      <input :value="modelValue"
             :placeholder="placeholder"
             :style="style"
             :maxlength="max_length"
             :class="{'input': true, 'err': !valid}"
             :readonly="readonly"
             @input="$emit('update:modelValue', $event.target.value)"
      />
    </div>
    <charactersLengthInfo v-if="max_length && counter" 
                          :readonly="readonly" 
                          :max_length="max_length" 
                          :value="modelValue.length"
                          :class="{'chars-err': !valid}"
                          style="margin-top:2px;"
    />
  </div>
</template>

<style scoped lang="stylus">
.input-container {
  box-sizing: border-box

  .label {
    display: block
    margin-bottom:10px
    color: rgba(255, 255, 255, 0.6)
    font-family: 'SFProDisplay', sans-serif
    font-size: 14px

    &.err {
      color: rgba(255, 98, 92, 0.7)
    }
  }

  .input {
    display: flex

    & > img {
      position: absolute
      padding: 15px 15px
    }
  }

  input {
    font-family: 'SFProDisplay', sans-serif
    background-color: rgba(255, 255, 255, 0.08)
    border: none
    outline-width: 0
    font-size: 14px
    color: white
    border-radius: 10px
    padding: 12px 8px
    width: 100%

    &:read-only {
      background-color: rgba(255, 255, 255, 0.03)
      color: rgba(255, 255, 255, 0.3)
    }

    &:not(.err):not(:read-only):focus {
      background-color: rgba(255, 255, 255, 0.12)
    }

    &.err:focus {
      background-color: rgba(255, 98, 92, 0.12)
    }

    &.err {
      color: rgba(255, 98, 92, 1)
      background-color: rgba(255, 98, 92, 0.07)
    }

    &:not(.err)::placeholder {
      font-size: 14px
      color: rgba(255, 255, 255, 0.3)
    }

    &.err::placeholder {
      font-size: 14px
      color: rgba(255, 98, 92, 0.4)
    }
  }
  .chars-err {
    color: rgba(255, 98, 92, 0.7)
  }
}
</style>

<script>
import charactersLengthInfo from './characters-length-info.vue'

export default {
  components: {
    charactersLengthInfo
  },
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
    valid: {
      type: Boolean,
      default: true,
      required: false
    },
    img: {
      type: String,
      default:'',
      required: false,
    },
    max_length:{
      type: Number,
      default: 10,
      required: true
    },
    counter: {
      type: Boolean,
      default: true,
      required: false
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
