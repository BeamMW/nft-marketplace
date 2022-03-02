<template>
    <div class="balance-container">
        <div :class="[show_balance ? '' : 'hidden', 'balance']">
            <img src="~assets/icon-beam.svg"/>
            <div class="value">
                {{ balance_beam }} BEAM
            </div>
            <div v-if="balance_beam" v-on:click="onWithdraw" class="withdraw">
                <img class="icon" src="~assets/icon-receive.svg"/>
                <span class="text">withdraw</span>
            </div>
        </div>
        <div class="user-opt">
            <div v-if="my_artist_name" class="user" v-on:click="onShowKey">
                <img class="user__icon" src="~assets/icon-user.svg"/>
                <span class="user__text">{{ my_artist_name }}</span>
            </div>
            <div class="key" v-on:click="onShowKey">
                <img class="key__icon" src="~assets/icon-key.svg"/>
                <span class="key__text">Show my public key</span>
            </div>
        </div>
    </div>
</template>

<style lang="stylus" scoped>
.balance-container {
    display: flex
    flex-direction: row
    justify-content: space-between
    margin-bottom: 10px

    .balance {
        width: 442px
        height: 78px
        border-radius: 10px
        background-color: rgba(255,255,255,0.1)
        display: flex
        flex-direction: row
        align-items: center
        padding: 0 20px
        padding-left: 20px

        .value {
            margin-left: 10px
            font-size: 20px
            font-weight: bold
            color: #fff
        }

        .withdraw {
            margin-left: auto
            display: flex
            align-items: center
            cursor: pointer
        }

        .icon {
            width: 16px
            height: 16px
        }

        .text {
            margin-left: 8px
            font-size: 14px
            font-weight: bold
            color: #0bccf7
        }
    }

    .user-opt {
        display: flex
        flex-direction: column
        margin-left: auto

        .user {
            padding-left: 15px
            padding-bottom: 12px
            display: flex
            align-items: center
            
            .user__text {
                font-size: 18px
                font-weight: bold
                color: #fff
                margin-left: 7px
            }
        }

        .key {
            padding: 14px
            border-radius: 10px
            background-color: rgba(255, 255, 255, .1)
            display: flex
            align-items: center
            cursor: pointer

            .key__text {
                font-size: 16px
                color: #00f6d2
                margin-left: 10px
            }

            .key__connect {
                font-size: 16px
                color: #00f6d2
            }
        }
    }
}
</style>

<script>
import { common, popups } from '../utils/consts.js';

export default {
    computed: {
        balance_beam () {
            return this.$state.balance_beam / common.GROTHS_IN_BEAM;
        },
        my_artist_name () {
            let artist = this.$state.artists[this.$state.my_artist_keys[0]] ||
                         this.$state.artists[this.$state.my_artist_keys[1]]
            return (artist || {}).label
        },
        show_balance () {
            return this.$state.is_artist || this.$state.balance_beam
        }
    },

    methods: {
        async onShowKey () {
            this.$store.setPopupType(popups.KEY)
            this.$store.changePopupState(true)
        },

        onWithdraw () {
            this.$store.withdrawBEAM()
        }
    }
}
</script>
