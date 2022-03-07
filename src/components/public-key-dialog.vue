<template>
    <modal :popupType="allPopups.KEY" ref="modal">
        <template v-slot:dialog="{ onClose }">
        <div class="key__title">
        Public key
        </div>

        <div class="key__data">
            <span>{{ display_artist_key }}</span>
            <img class="copy-icon" v-on:click="onCopy" src="~assets/icon-copy.svg"/>
        </div>

        <div class="key__controls">
            <artButton 
            v-on:click="onClose"
            type="close"/>

            <artButton
            data="display_artist_key"
            v-on:click="onCopy"
            type="copy"/>
        </div>
        </template>
    </modal>
</template>

<style scoped lang="stylus">
.key__title {
    font-size: 18px
    font-weight: bold
    color: #fff
}
.key__data {
    margin-top: 30px
    display: flex
    flex-direction: row
    align-items: center

    .copy-icon {
        margin-left: 10px
        cursor: pointer
    }
}

.key__controls {
    margin-top: 30px
    display: flex
    flex-direction: row
}

</style>

<script>
import modal from './modal.vue'
import artButton from './art-button.js';
import {  popups } from '../utils/consts.js';

export default {

      components: { 
        modal, artButton
    },
    
    data () {
        return {
            allPopups: popups,
        }
    },

    computed: {
        display_artist_key () {
            return this.$state.my_artist_keys[0];
        },
    },
    methods: {
        onCopy() {
            var textArea = document.createElement("textarea");
            textArea.style.position = "fixed";
            textArea.value = this.display_artist_key;
            document.body.appendChild(textArea);
            textArea.focus();
            textArea.select();
            this.$refs.modal.onClose();
            try {
                return document.execCommand("copy");
            } catch (ex) {
                return false;
            } finally {
                document.body.removeChild(textArea);
            }
        }
    }
}
</script>