<template>
  <div class="input-container">
    <label v-if="label" class="label" :class="{'err': !valid}">
      {{ label }}
    </label>
    <div class="input" :class="{'err': !valid, 'readonly': readonly}">
      <img v-if="img" :src="icon_src" :class="{'error': !valid}" alt="icon"/>
      <input ref="input"
             :value="modelValue"
             :placeholder="placeholder"
             :style="style"
             :maxlength="max_length"
             :class="{'err': !valid}"
             :readonly="readonly"
             @input="$emit('update:modelValue', $event.target.value)"
             @keydown="onKeyDown"
             @contextmenu="onContextMenu"
      />
      <slot></slot>
      <popupMenu ref="ctxMenu">
        <div class="item" @click="onCut">
          <img src="~assets/remove.svg"/>
          cut
        </div>
        <div class="item" @click="onCopy">
          <img src="~assets/copy.svg"/>
          copy
        </div>
        <div class="item" @click="onPaste">
          <img src="~assets/change.svg"/>
          paste
        </div>
      </popupMenu>
    </div>
    <charslen v-if="max_length && counter" 
              :readonly="readonly" 
              :max_length="max_length" 
              :value="modelValue.length"
              :class="{'chars-err': !valid}"
              :tip="tip"
              style="margin-top:3px;"
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
    background-color: rgba(255, 255, 255, 0.08)
    border: none
    border-radius: 10px
    align-items: center
    position: relative

    & > img {
      position: absolute
      padding: 15px 15px
    }

    &.readonly {
      background-color: rgba(255, 255, 255, 0.03)
    }

    &.err {
      background-color: rgba(255, 98, 92, 0.07)
      &:focus-within {
        background-color: rgba(255, 98, 92, 0.12)
      }
    }

    &:not(.err):not(.readonly):focus-within {
      background-color: rgba(255, 255, 255, 0.12)
    }

    & > input {
      font-family: 'SFProDisplay', sans-serif
      background-color: transparent
      border: none
      border-radius: 10px
      outline-width: 0
      font-size: 14px
      color: rgba(255, 255, 255, 0.8)
      padding: 10px 8px
      width: 100%

      &:read-only {
        color: rgba(255, 255, 255, 0.3)
      }

      &.err {
        color: rgba(255, 98, 92, 1)
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
  }

  .chars-err {
    color: rgba(255, 98, 92, 0.7)
  }
}
</style>

<script>
import {nextTick} from 'vue'
import utils from 'utils/utils'
import charslen from 'controls/charslen'
import popupMenu from 'controls/popup-menu'
import validators from 'utils/validators'

export default {
  components: {
    charslen,
    popupMenu
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
      default: undefined,
      required: false
    },
    counter: {
      type: Boolean,
      default: true,
      required: false
    },
    tip: {
      type: String,
      default: undefined,
      required: false
    },
    allowed: {
      type: Object,
      default: validators.text_allowed(),
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
    },
    icon_src() {
      return require('assets/' + this.img + '.svg')
    }
  },

  methods: {
    onKeyDown(ev) {
      // TODO: merge 'allowed', 'valid' & 'max_chars' to one regex
      if (this.allowed && !this.allowed.test(ev.key)) {
        ev.preventDefault()
      }
    },

    // the desktop wallet webview has no native context menu
    onContextMenu(ev) {
      ev.preventDefault()
      this.$refs.input.focus()
      this.$refs.ctxMenu.open(ev)
    },

    selection() {
      let input = this.$refs.input
      let from = input.selectionStart
      let to = input.selectionEnd
      return {from, to, text: String(this.modelValue).substring(from, to)}
    },

    replaceSelection(text) {
      let input = this.$refs.input
      let value = String(this.modelValue)
      let from = input.selectionStart
      let to = input.selectionEnd
      let next = value.substring(0, from) + text + value.substring(to)

      if (this.max_length && next.length > this.max_length) {
        next = next.substring(0, this.max_length)
      }

      this.$emit('update:modelValue', next)

      nextTick(() => {
        let pos = Math.min(from + text.length, next.length)
        input.setSelectionRange(pos, pos)
      })
    },

    async onCopy() {
      this.$refs.ctxMenu.close()
      let {text} = this.selection()
      if (text) {
        await utils.copyText(text)
      }
    },

    async onCut() {
      this.$refs.ctxMenu.close()

      if (this.readonly) {
        return
      }

      let {text} = this.selection()
      if (!text) {
        return
      }

      await utils.copyText(text)
      this.replaceSelection('')
    },

    async onPaste() {
      this.$refs.ctxMenu.close()

      if (this.readonly) {
        return
      }

      let text = await utils.pasteText()
      if (!text) {
        return
      }

      if (this.allowed && Array.from(text).some(ch => !this.allowed.test(ch))) {
        return
      }

      this.replaceSelection(text)
    }
  }
}
</script>
