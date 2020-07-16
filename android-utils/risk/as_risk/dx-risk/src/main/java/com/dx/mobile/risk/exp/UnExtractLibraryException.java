package com.dx.mobile.risk.exp;

public class UnExtractLibraryException extends LinkageError {
    public UnExtractLibraryException(final String library) {
        super(" couldn't extract " + library + " from the apk!");
    }
}