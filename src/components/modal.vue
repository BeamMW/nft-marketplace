<template>
    <div class="popup" id="popup">
         <div  :class="[popupType === allPopups.KEY ? 'content-key' : 'content-sell']"
                id="popup-content"   
        >
        <slot name="dialog"  :onClose="onClose"></slot>
         </div>
    </div>
</template>

<style lang="stylus" scoped>
.popup {
    width: 100%
    height: 100%
    overflow: hidden
    position: fixed
    top: 0px
    display: flex
    justify-content: center
    align-items: center
    z-index: 100
    
    .content-key {
        padding: 40px
        border-radius: 10px
        display: flex
        flex-direction: column
        align-items: center
        width: 760px
    }

    .content-sell {
        padding: 40px
        border-radius: 10px
        display: flex
        flex-direction: column
        align-items: center
        width: 390px
    }
}
</style>

<script>
import { popups } from '../utils/consts.js';
import utils from '../utils/utils.js';
import artButton from './art-button.js';

export default {

    components: {
        artButton
    },
    props: ['popupType'],

    data() {
        return {
        allPopups: popups,
        }
    },

    mounted: function () {
        this.applyStyles();
    },

    unmounted: function () {
        this.removeStyles()
    },

    methods: {
        applyStyles() {
            const { background_popup } = utils.getStyles()
            document.getElementById("popup-content").style.backgroundColor = background_popup
            document.getElementById("container").style.opacity = 0.3
        },

        removeStyles() {
            document.getElementById("container").style.opacity = 1
        },

        onClose() {
            this.$store.changePopupState(false)
        },
    }
}

</script>