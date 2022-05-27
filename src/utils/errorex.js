//
// Since Qt 5.12 uses chromium 69 that doesn't support error.cause
// we emulate it. Should switch to standard Error as soon as
// Qt & bundled chromium are updated
//
export default class ErrorEx extends Error {
  constructor(message, cause) {
    super(message)
    this.cause = cause
  }
}
