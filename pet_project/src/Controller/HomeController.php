<?php

namespace App\Controller;

use App\Operation\EmployeeOperation;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;

class HomeController extends AbstractController
{
    private EmployeeOperation $employeeOperation;

    public function __construct(EmployeeOperation $employeeOperation)
    {
        $this->employeeOperation = $employeeOperation;
    }

    #[Route('/', name: 'home_index')]
    public function index(): Response
    {
        $errorMessage = null;

        try {
            $latestUsersList = $this->employeeOperation->findPaginated(5, 0);
            $allRoles = $this->employeeOperation->findAllRoles();
        } catch (\Exception $e) {
            $errorMessage = 'Tato stránka zatím neexistuje.';
            $latestUsersList = [];
            $allRoles = [];
        }

        return $this->render('home/index.html.twig', [
            'latestUsers' => $latestUsersList,
            'allRoles' => $allRoles,
            'errorMessage' => $errorMessage,
        ]);
    }
}
