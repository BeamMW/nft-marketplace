<template>
  <div class="asset-select">
    <label v-if="label" class="label">{{ label }}</label>

    <button type="button" class="selector" :disabled="readonly" @click="open">
      <assetIcon :aid="modelValue" :size="20"/>
      <span class="text">{{ selected.unit_name }}</span>
      <span class="aid">id:{{ modelValue }}</span>
      <img src="~assets/arrow-down.svg" class="chevron"/>
    </button>

    <modal ref="modal">
      <div class="picker">
        <div class="picker-title">Select asset</div>

        <input ref="search"
               v-model="search"
               class="picker-search"
               placeholder="Search by name or asset id..."
               maxlength="40"
        />

        <div class="picker-list">
          <div v-for="asset in filtered"
               :key="asset.aid"
               class="picker-item"
               :class="{'active': asset.aid === modelValue}"
               @click="onSelect(asset.aid)"
          >
            <assetIcon :aid="asset.aid" :size="24"/>
            <div class="picker-item-text">
              <div class="picker-item-name">{{ asset.unit_name }}</div>
              <div class="picker-item-sub">{{ asset.name }}</div>
            </div>
            <div class="picker-item-aid">id:{{ asset.aid }}</div>
          </div>

          <!-- typing an id we have never seen still has to be selectable -->
          <div v-if="manual_aid !== undefined" class="picker-item" @click="onSelect(manual_aid)">
            <assetIcon :aid="manual_aid" :size="24"/>
            <div class="picker-item-text">
              <div class="picker-item-name">Use asset #{{ manual_aid }}</div>
              <div class="picker-item-sub">not seen in this gallery yet</div>
            </div>
          </div>

          <div v-if="!filtered.length && manual_aid === undefined" class="picker-empty">
            No assets match "{{ search }}"
          </div>
        </div>

        <div class="picker-controls">
          <btn text="cancel" @click="close">
            <img src="~assets/cancel.svg"/>
          </btn>
        </div>
      </div>
    </modal>
  </div>
</template>

<style scoped lang="stylus">
.asset-select {
  display: flex
  flex-direction: column

  & > .label {
    display: block
    margin-bottom: 10px
    color: rgba(255, 255, 255, 0.6)
    font-family: 'SFProDisplay', sans-serif
    font-size: 14px
  }

  & > .selector {
    display: flex
    align-items: center
    box-sizing: border-box
    width: 100%
    height: 41px
    padding: 0 12px 0 14px
    background-color: rgba(255, 255, 255, 0.05)
    border: 1px solid rgba(255, 255, 255, 0.08)
    border-radius: 170px
    cursor: pointer
    color: #fff
    font-family: 'SFProDisplay', sans-serif
    font-size: 14px
    outline: none

    &:hover:not([disabled]) {
      background-color: rgba(255, 255, 255, 0.08)
      border-color: rgba(255, 255, 255, 0.15)
    }

    & > .text {
      flex: 1
      min-width: 0
      margin-left: 8px
      text-align: left
      white-space: nowrap
      overflow: hidden
      text-overflow: ellipsis
    }

    & > .aid {
      margin-left: 8px
      color: rgba(255, 255, 255, 0.5)
    }

    & > .chevron {
      width: 10px
      margin-left: 10px
      opacity: 0.5
      flex-shrink: 0
    }
  }
}

.picker {
  display: flex
  flex-direction: column
  width: 380px
  max-width: 80vw

  & > .picker-title {
    font-size: 14px
    font-weight: bold
    text-transform: uppercase
    letter-spacing: 2px
    color: #fff
    text-align: center
  }

  & > .picker-search {
    box-sizing: border-box
    width: 100%
    margin-top: 20px
    padding: 10px 14px
    background-color: rgba(255, 255, 255, 0.05)
    border: 1px solid rgba(255, 255, 255, 0.08)
    border-radius: 10px
    color: #fff
    font-family: 'SFProDisplay', sans-serif
    font-size: 14px
    outline: none

    &::placeholder {
      color: rgba(255, 255, 255, 0.4)
    }

    &:focus {
      border-color: #00f6d2
    }
  }

  & > .picker-list {
    margin-top: 14px
    max-height: 280px
    overflow-y: auto

    & > .picker-item {
      display: flex
      align-items: center
      padding: 10px 12px
      border-radius: 10px
      cursor: pointer

      &:hover {
        background-color: rgba(255, 255, 255, 0.06)
      }

      &.active {
        background-color: rgba(0, 246, 210, 0.1)
      }

      & > .picker-item-text {
        flex: 1
        min-width: 0
        margin-left: 12px

        & > .picker-item-name {
          font-size: 14px
          color: #fff
          white-space: nowrap
          overflow: hidden
          text-overflow: ellipsis
        }

        & > .picker-item-sub {
          margin-top: 2px
          font-size: 12px
          color: rgba(255, 255, 255, 0.5)
          white-space: nowrap
          overflow: hidden
          text-overflow: ellipsis
        }
      }

      & > .picker-item-aid {
        margin-left: 10px
        font-size: 12px
        color: rgba(255, 255, 255, 0.5)
      }
    }

    & > .picker-empty {
      padding: 20px
      text-align: center
      font-size: 14px
      color: rgba(255, 255, 255, 0.5)
    }
  }

  & > .picker-controls {
    display: flex
    justify-content: center
    margin-top: 20px
  }
}
</style>

<script>
import {nextTick} from 'vue'
import modal from 'controls/modal'
import btn from 'controls/button'
import assetIcon from 'controls/asset-icon'
import assetsStore from 'stores/assets'

export default {
  components: {
    modal, btn, assetIcon
  },

  props: {
    // eslint-disable-next-line vue/prop-name-casing
    modelValue: {
      type: Number,
      default: 0
    },
    label: {
      type: String,
      default: ''
    },
    readonly: {
      type: Boolean,
      default: false
    }
  },

  emits: [
    'update:modelValue'
  ],

  data () {
    return {
      search: ''
    }
  },

  computed: {
    selected () {
      return assetsStore.get(this.modelValue)
    },

    filtered () {
      let needle = this.search.trim().toLowerCase()
      let list = assetsStore.known

      if (!needle) {
        return list
      }

      return list.filter(asset => {
        return String(asset.unit_name).toLowerCase().indexOf(needle) !== -1 ||
               String(asset.name).toLowerCase().indexOf(needle) !== -1 ||
               String(asset.aid) === needle
      })
    },

    // An unknown numeric id becomes an explicit row rather than no results.
    manual_aid () {
      let needle = this.search.trim()
      if (!/^[0-9]+$/.test(needle)) {
        return undefined
      }

      let aid = parseInt(needle, 10)
      if (this.filtered.some(asset => asset.aid === aid)) {
        return undefined
      }

      return aid
    }
  },

  methods: {
    open () {
      if (this.readonly) {
        return
      }

      this.search = ''
      this.$refs.modal.open()
      nextTick(() => {
        if (this.$refs.search) this.$refs.search.focus()
      })
    },

    close () {
      this.$refs.modal.close()
    },

    onSelect (aid) {
      assetsStore.noteAid(aid)
      this.$emit('update:modelValue', aid)
      this.close()
    }
  }
}
</script>
