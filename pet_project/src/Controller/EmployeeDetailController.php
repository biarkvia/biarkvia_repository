<?php

namespace App\Controller;

use App\Operation\EmployeeOperation;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;

class EmployeeDetailController extends AbstractController
{
    private EmployeeOperation $employeeOperation;

    public function __construct(EmployeeOperation $employeeOperation)
    {
        $this->employeeOperation = $employeeOperation;
    }

    #[Route('/employees/detail/{id}', name: 'employee_detail')]
    public function displayEmployee(int $id): Response
    {
        $employee = $this->employeeOperation->findById($id);

        if (!$employee) {
            return $this->redirectToRoute('error', [
                'message' => 'Zaměstnanec nebyl nalezen.',
            ]);
        }

        return $this->render('detail/employee_detail.html.twig', [
            'employee' => $employee,
            'accounts' => $employee->getAccounts(),
        ]);
    }
}
