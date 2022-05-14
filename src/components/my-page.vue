<template>
  <div class="my-page">
    <pageTitle title="my page">
      <div class="actions">
        <btnEditArtist/>
        <btnWallet/>
        <btnKey/>
      </div>
    </pageTitle>
    <myGallery class="gallery"/>
  </div>
</template>

<style scoped lang="stylus">
  .my-page {
    width: 100%
    height: 100%
    display: flex
    flex-direction: column

    & > .gallery {
      flex: 1
      box-sizing: border-box
    }

    .actions {
      display: flex
      justify-content: flex-end
      align-items: center

      & > * {
        margin-left: 12px
        margin-top: 7px

        &:last-child {
          margin-right: 7px
        }
      }
    }
  }
</style>

<script>
import pageTitle from './page-title.vue'
import btnWallet from './button-wallet'
import btnEditArtist from './btn-edit-artist'
import btnKey from './button-key'
import myGallery from './my-gallery'

export default {
  components: {
    pageTitle,
    btnEditArtist,
    btnWallet,
    btnKey,
    myGallery
  }
}
</script>
