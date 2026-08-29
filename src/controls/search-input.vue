<template>
  <div class="search-input">
    <div class="input-container">
      <div class="icon">
        <img v-if="modelValue"
             src="~assets/cancel.svg"
             alt="clear"
             class="clear"
             @mousedown.prevent="onClear"
        />
        <img v-else src="~assets/search.svg" alt="search"/>
      </div>
      <input ref="input"
             :value="modelValue"
             :maxlength="max_length"
             :placeholder="placeholder"
             class="input"
             @input="onInput"
             @focus="focused = true"
             @blur="onBlur"
             @keydown.down.prevent="move(1)"
             @keydown.up.prevent="move(-1)"
             @keydown.esc="close"
             @keyup.enter="onEnter"
      />
    </div>

    <div v-if="show_suggestions" class="suggestions">
      <div v-for="(item, idx) in suggestions"
           :key="`${item.type}-${item.id}`"
           class="suggestion"
           :class="{'active': idx === highlight}"
           @mousedown.prevent="onSelect(item)"
           @mouseenter="highlight = idx"
      >
        <preview class="kind"
                 :image="item.image"
                 :default="item.default_image"
                 :show_text="false"
                 width="28px"
                 height="28px"
                 :radius="item.type === 'artist' ? '14px' : '6px'"
                 cover
        />
        <div class="text">
          <div class="label">{{ item.label }}</div>
          <div class="sub">{{ item.sub }}</div>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .search-input {
    position: relative
  }

  /* matches the BANS search field (Input variant='proposal') */
  .input-container {
    display: flex
    position: relative
    width: 100%
    height: 45px

    & > .icon {
      position: absolute
      top: 0
      right: 0
      bottom: 0
      display: flex
      align-items: center
      justify-content: flex-end
      padding-right: 15px
      pointer-events: none

      & > * {
        pointer-events: auto
      }

      & > .clear {
        cursor: pointer
        opacity: 0.7

        &:hover {
          opacity: 1
        }
      }
    }

    .input {
      box-sizing: border-box
      font-family: 'SFProDisplay', sans-serif
      background-color: rgba(255, 255, 255, 0.05)
      border: 1px solid rgba(255, 255, 255, 0.5)
      border-radius: 10px
      outline-width: 0
      font-size: 14px
      line-height: 17px
      color: white
      padding: 0 45px 0 15px
      width: 100%
      overflow: hidden
      text-overflow: ellipsis
      white-space: nowrap

      &::placeholder {
        font-size: 16px
        font-style: italic
        color: white
        opacity: 0.5
      }
    }
  }

  /* same panel treatment as the dropdowns */
  .suggestions {
    position: absolute
    top: 100%
    left: 0
    z-index: 10
    box-sizing: border-box
    width: 100%
    margin-top: 6px
    padding: 6px 0
    max-height: 280px
    overflow-y: auto
    background-color: #042548
    border: 1px solid rgba(255, 255, 255, 0.08)
    border-radius: 10px
    box-shadow: 0 8px 32px rgba(0, 0, 0, 0.5)

    & > .suggestion {
      display: flex
      align-items: center
      padding: 8px 14px
      cursor: pointer

      &.active {
        background-color: rgba(255, 255, 255, 0.07)
      }

      & > .kind {
        flex-shrink: 0
      }

      & > .text {
        min-width: 0
        margin-left: 10px

        & > .label {
          font-size: 14px
          color: #fff
          white-space: nowrap
          overflow: hidden
          text-overflow: ellipsis
        }

        & > .sub {
          margin-top: 2px
          font-size: 12px
          color: rgba(255, 255, 255, 0.5)
        }
      }
    }
  }
</style>

<script>
import preview from 'controls/preview'

export default {
  components: {
    preview
  },

  props: {
    // eslint-disable-next-line vue/prop-name-casing
    modelValue: {
      type: String,
      default: '',
      required: true
    },
    max_length:{
      type: Number,
      default: 10,
      required: true
    },
    placeholder: {
      type: String,
      default: '',
      required: true
    },
    // [{id, label, sub, type, image}] - resolved by the parent
    suggestions: {
      type: Array,
      default: () => []
    }
  },

  emits: [
    'update:modelValue',
    'search',
    'select'
  ],

  data () {
    return {
      focused: false,
      highlight: -1
    }
  },

  computed: {
    show_suggestions () {
      return this.focused && this.suggestions.length > 0
    }
  },

  watch: {
    suggestions () {
      this.highlight = -1
    }
  },

  methods: {
    onInput (ev) {
      this.focused = true
      this.$emit('update:modelValue', ev.target.value)
    },

    onBlur () {
      // mousedown on a suggestion is handled before blur closes the list
      this.focused = false
      this.highlight = -1
    },

    close () {
      this.focused = false
      this.highlight = -1
    },

    move (delta) {
      if (!this.suggestions.length) {
        return
      }

      this.focused = true
      let next = this.highlight + delta
      if (next < 0) next = this.suggestions.length - 1
      if (next >= this.suggestions.length) next = 0
      this.highlight = next
    },

    onEnter () {
      if (this.highlight >= 0 && this.suggestions[this.highlight]) {
        return this.onSelect(this.suggestions[this.highlight])
      }
      this.onSubmit()
    },

    onSelect (item) {
      this.close()
      this.$refs.input.blur()
      this.$emit('select', item)
    },

    onClear() {
      this.$emit('update:modelValue', '')
      this.close()
      this.$refs.input.focus()
    },

    onSubmit() {
      this.close()
      this.$emit('search', this.modelValue)
    }
  }
}
</script>
