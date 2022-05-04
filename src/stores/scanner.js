class Scanner {
  reset (global) {    
    this._global = global
  }

  async loadAsync() { /*
    if (this._db) {
      // TODO: this happens on app start, need to investigate
      return
    }

    await this._open()
    
    this._chainColls = new ChainColls(this)
    this._chainColls.start()
    */
  }
}

let scanner = new Scanner()
export default scanner