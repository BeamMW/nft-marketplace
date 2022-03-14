<template>
    <modal ref="modal">
        <template v-slot:dialog="{ onClose }">
                <div class="sell__title">
                    Set the price
                </div>

                <div class="sell__input">
                    <input type="text"
                    id="sell-input"
                    class="input__elem" 
                    placeholder="0"/>
                    <span class="input__text">BEAM</span>
                </div>

                <div class="sell__fee">
                    <span class="fee__title">Fee</span>
                    <span class="fee__value">0.011 BEAM</span>
                </div>

                <div class="sell__info">
                    Small transaction fee must be paid
                </div>

                <div class="sell__controls">
                    <artButton
                    v-on:click="onClose"
                    type="cancel"
                    />

                    <artButton
                    v-on:click="onProceed"
                    id="proceed"
                    type="proceed"
                    class="disabled"
                    />
                </div>
        </template>
    </modal>
</template>

<style scoped lang="stylus">
.sell__title {
    font-size: 18px
    font-weight: bold
    color: #fff
}

.sell__input {
    margin-top: 20px
    height: 45px
    width: 100%
    border-radius: 10px
    background-color: rgba(255, 255, 255, .05)
    display: flex
    flex-direction: row
    align-items: center

    .input__elem {
        background: rgba(255, 255, 255, 0)
        border: none
        font-size: 24px
        resize: none
        color: #0bccf7
        height: 100%
        padding: 0
        border-radius: 10px
        padding: 8px
        max-width: 292px
        
        &:focus {
            outline-width: 0
            color: #0bccf7
        }

        &::placeholder {
            color: #0bccf7
        }
    }

    .input__text {
        margin-right: 8px
        font-size: 16px
        margin-left: auto
        color: #ffffff
    }

}
.sell__fee {
    display: flex
    flex-direction: row
    margin-top: 20px
    align-self: flex-start
    
    .fee__title {
        opacity: 0.5
        font-size: 12px
        color: #fff
    }

    .fee__value {
        font-size: 14px
        font-weight: 500
        color: #0bccf7
        margin-left: 10px
    }
}

.sell__info {
    margin-top: 30px
    opacity: 0.7
    font-size: 14px
    font-style: italic
    color: #fff
}

.sell__controls {
    margin-top: 30px
    display: flex
    flex-direction: row
}

.disabled {
    opacity: 0.3
    cursor: auto !important
}
</style>

<script>
import modal from './modal.vue'
import artButton from './art-button.js';

export default {
    components: { 
        modal, artButton
    },

    mounted () { 
        const sellInput = document.getElementById('sell-input');
        if (sellInput) {
            document.getElementById('sell-input').addEventListener('keydown', (event) => {
                const specialKeys = [
                    'Backspace', 'Tab', 'ArrowDown', 'ArrowLeft', 'ArrowRight', 'ArrowUp',
                    'Control', 'Delete', 'F5'
                ];

                if (specialKeys.indexOf(event.key) !== -1) {
                    return;
                }

                const current = document.getElementById('sell-input').value;
                const next = current.concat(event.key);
            
                if (!utils.handleString(next)) {
                    event.preventDefault();
                }
            })

            document.getElementById('sell-input').addEventListener('paste', (event) => {
                if (event.clipboardData !== undefined) {
                    const text = event.clipboardData.getData('text');
                    if (!utils.handleString(text)) {
                        event.preventDefault();
                    }
                }
            });

            sellInput.addEventListener('input', ev => {
                let disabled = parseFloat(ev.target.value || '0') == 0
                let proceed = document.getElementById('proceed')
                
                if (disabled) {
                    proceed.classList.add('disabled')
                } 
                
                if (!disabled) {
                    proceed.classList.remove('disabled')   
                }
            })
        }
    },

    computed: {
        id_to_sell () {
            return this.$state.id_to_sell;
        }
    },

    methods: {
        onProceed() {
            let proceed = document.getElementById('proceed')
            if (!proceed.classList.contains('disabled')) {
                const current = document.getElementById('sell-input').value;
                const price = parseFloat(current) * common.GROTHS_IN_BEAM;
                this.$store.sellArtwork(this.id_to_sell, price);
                this.onClose();
            }
        },
    }
}
</script>