<template>
  <messageModal ref="modal"/>
</template>

<script>
import messageModal from 'components/message-modal'

export default {
  components: {
    messageModal
  },
  methods: {
    open() {
      this.$refs.modal.open('Image is too large', 'The image you are trying to upload is too large. Please upload an image that is less than 500KB.')
    }
  }
}
</script>
