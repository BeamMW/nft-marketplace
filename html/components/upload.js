import html from '../utils/html.js'
import utils from '../utils/utils.js'

export default {
    render (...args) {
        return html`
            <div class="upload">
                <input type="file" accept="image/png, image/jpeg" onChange=${this.previewFile}></input>
            </div>
        `
    },

    methods: {
        previewFile (ev) {
            let file = ev.target.files[0]
            if (!file) return

            let name = file.name.split('.')[0]
            name = [name[0].toUpperCase(), name.substring(1)].join('')
            
            let reader = new FileReader()
            reader.readAsArrayBuffer(file)

            reader.onload = ()=> {
                let aver  = Uint8Array.from([1])
                let aname = (new TextEncoder()).encode(name)
                let asep  = Uint8Array.from([0, 0])
                let aimg  = new Uint8Array(reader.result)
                let hex   = [utils.hexEncodeU8A(aver), 
                             utils.hexEncodeU8A(aname), 
                             utils.hexEncodeU8A(asep), 
                             utils.hexEncodeU8A(aimg)
                            ].join('')
                alert(`Uploading image\nName: ${name}\nData size: ${hex.length}`)
                utils.invokeContract(`role=artist,action=upload,cid=${this.$root.cid},data=${hex}`, (...args) => this.onUpload(...args))
            }
        },

        onUpload(err, sres, full) {
            if (err) {
                return this.$root.setError(err, "Failed to generate upload request")
            }

            utils.ensureField(full.result, "raw_data", "array")
            utils.callApi('process_invoke_data', {data: full.result.raw_data}, (...args) => this.onSendToChain(...args))
        },

        onSendToChain (err, resp) {
            if (err) {
                alert("error")
                return this.$root.setError(err, "Failed to upload image")
            }
            //alert(utils.formatJSON(resp))
        }
    }
}
