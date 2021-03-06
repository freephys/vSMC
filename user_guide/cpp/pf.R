library(ggplot2)

pf <- read.table("pf.out", header = TRUE)
sink("pf.rout")
print(pf[1:5,])
sink()

obs <- read.table("pf.data", header = FALSE)
dat <- data.frame(
    X = c(pf[["pos.0"]], obs[,1]),
    Y = c(pf[["pos.1"]], obs[,2]))
dat[["Source"]] <- rep(c("Estimate", "Observation"), each = dim(obs)[1])
plt <- qplot(x = X, y = Y, data = dat, geom = "path")
plt <- plt + aes(group = Source, color = Source, linetype = Source)
plt <- plt + theme_bw() + theme(legend.position = "top")
pdf("pf.pdf")
print(plt)
dev.off()
