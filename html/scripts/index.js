import Utils from "./utils.js"

const to_def = 3000
const id_err = 'error'

class Gallery {

    constructor(beam) {    
        this.beam      = beam
        this.style     = beam.style
        this.api       = beam.api
        this.cid       = "0e982209bf4202075fa4c4acd5b43e7b559112e5b7ffe5b78f8ebd88e3c07609"
        Utils.getById(id_err).style.color = this.style.validator_error
    }

    setError (err, context) {
        let errmsg = [context || "Error occured",  Utils.formatJSON(err)].join(':\n')
        Utils.setText(id_err, errmsg)
        Utils.show(id_err)
        this.restart()
    }

    restart (now) {
        if (this.timeout) clearTimeout(this.timeout)
        this.timeout = setTimeout(() => this.start(), now ? 0 : to_def)
    }

    onApiResult(err, res, full) {
        if (err) return this.setError(err,  "API handling error")
        this.setError(full, "Unexpected API result")
    }

    start () { 
        Utils.download("./galleryManager.wasm", (err, bytes) => {
            if (err) {
                return this.onError(err, "Shader download")
            }
            this.shader = bytes
            Utils.invokeContract("checkCID", "role=manager,action=view", this.shader)
        })
    }

    onShowMethods(err, res) {
        if (err) return this.setError(err)
        alert(Utils.formatJSON(res))
    }

    onCheckCID (err, res) {
        if (err) return this.setError(err)     
        if (!res.contracts.some(el => el.cid == this.cid)) {
            throw `Failed to verify cid '${this.cid}'`
        }
        Utils.invokeContract("showMethods")
    }
}

Utils.onLoad(beam => {
    return new Gallery(beam)
})
