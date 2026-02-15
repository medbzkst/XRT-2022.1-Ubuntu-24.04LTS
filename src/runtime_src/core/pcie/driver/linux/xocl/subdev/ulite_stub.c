#include <linux/pci.h>

int xocl_init_ulite(struct pci_dev *pdev);
void xocl_fini_ulite(struct pci_dev *pdev);

/* UARTLite support removed for newer kernels.
 * Provide stubs so dependent modules link.
 */
int xocl_init_ulite(struct pci_dev *pdev)
{
    return 0;
}

void xocl_fini_ulite(struct pci_dev *pdev)
{
}