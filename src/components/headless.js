import html from "../utils/html.js";

export default {
  render() {
    return html`
      <div class="balance-container">
        <div class="user-opt">
          <div class="user-opt__key" onclick=${this.onConnect}>
            <span class="user-opt__key__connect">Connect to wallet</span>
          </div>
        </div>
      </div>
    `;
  },

  methods: {
    async onConnect() {
      this.$store.switchToHeaded();
    },
  },
};
